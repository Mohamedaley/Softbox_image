#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/rtnetlink.h>
#include "network.h"

#define VETH_PREFIX "veth"
#define CONTAINER_IFNAME "eth0"

int setup_container_network() {
    // Create a veth pair
    char veth_name[IFNAMSIZ];
    snprintf(veth_name, IFNAMSIZ, "%s%d", VETH_PREFIX, getpid());
    
    if (system("ip link add veth0 type veth peer name veth1") != 0) {
        perror("Failed to create veth pair");
        return -1;
    }

    // Move veth1 to container network namespace
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "ip link set veth1 netns %d", getpid());
    if (system(cmd) != 0) {
        perror("Failed to move veth1 to container namespace");
        return -1;
    }

    // Set up IP address for veth0 (host side)
    if (system("ip addr add 10.0.0.1/24 dev veth0") != 0) {
        perror("Failed to set IP for veth0");
        return -1;
    }

    // Set up IP address for veth1 (container side)
    if (system("ip netns exec $(basename `ip netns identify`) ip addr add 10.0.0.2/24 dev veth1") != 0) {
        perror("Failed to set IP for veth1");
        return -1;
    }

    // Bring up the interfaces
    if (system("ip link set veth0 up") != 0 ||
        system("ip netns exec $(basename `ip netns identify`) ip link set veth1 up") != 0) {
        perror("Failed to bring up interfaces");
        return -1;
    }

    // Set up NAT on the host
    if (system("iptables -t nat -A POSTROUTING -s 10.0.0.0/24 -j MASQUERADE") != 0) {
        perror("Failed to set up NAT");
        return -1;
    }

    return 0;
}