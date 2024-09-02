#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cgroups.h"

#define MAX_BUF 1024

int setup_container_cgroups(const char* container_id) {
    char buf[MAX_BUF];
    int fd;

    // Create cgroup
    snprintf(buf, sizeof(buf), "sudo mkdir -p /sys/fs/cgroup/memory/%s", container_id);
    system(buf);

    // Set memory limit (e.g., 100MB)
    snprintf(buf, sizeof(buf), "/sys/fs/cgroup/memory/%s/memory.limit_in_bytes", container_id);
    fd = open(buf, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    write(fd, "100000000", 9);
    close(fd);

    // Add current process to cgroup
    snprintf(buf, sizeof(buf), "/sys/fs/cgroup/memory/%s/cgroup.procs", container_id);
    fd = open(buf, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    snprintf(buf, sizeof(buf), "%d", getpid());
    write(fd, buf, strlen(buf));
    close(fd);

    return 0;
}