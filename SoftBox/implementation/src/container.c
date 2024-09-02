#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "container.h"
#include "namespace.h"
#include "cgroups.h"
#include "filesystem.h"
#include "network.h"

#define MAX_CONTAINERS 10
#define CONTAINER_ID_LENGTH 12

typedef struct {
    char id[CONTAINER_ID_LENGTH + 1];
    pid_t pid;
    char image[256];
} container_t;

static container_t containers[MAX_CONTAINERS];
static int container_count = 0;

static char* generate_container_id() {
    static char id[CONTAINER_ID_LENGTH + 1];
    const char charset[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    
    for (int i = 0; i < CONTAINER_ID_LENGTH; i++) {
        int index = rand() % (sizeof(charset) - 1);
        id[i] = charset[index];
    }
    id[CONTAINER_ID_LENGTH] = '\0';
    
    return id;
}

int run_container(char* image) {
    if (container_count >= MAX_CONTAINERS) {
        fprintf(stderr, "Error: Maximum number of containers reached\n");
        return 1;
    }

    char* container_id = generate_container_id();
    
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        return 1;
    }

    if (child_pid == 0) {
        // Child process
        if (setup_container_namespaces() == -1 ||
            setup_container_cgroups(container_id) == -1 ||
            setup_container_fs(image) == -1 ||
            setup_container_network() == -1) {
            exit(1);
        }

        char *args[] = {"/bin/bash", NULL};
        execvp(args[0], args);
        perror("execvp");
        exit(1);
    } else {
        // Parent process
        container_t new_container;
        strncpy(new_container.id, container_id, CONTAINER_ID_LENGTH + 1);
        new_container.pid = child_pid;
        strncpy(new_container.image, image, 256);
        containers[container_count++] = new_container;

        printf("Container started: %s\n", container_id);
        return 0;
    }
}

int list_containers() {
    printf("CONTAINER ID\tIMAGE\t\tPID\n");
    for (int i = 0; i < container_count; i++) {
        printf("%s\t%s\t\t%d\n", containers[i].id, containers[i].image, containers[i].pid);
    }
    return 0;
}

int stop_container(char* id) {
    for (int i = 0; i < container_count; i++) {
        if (strcmp(containers[i].id, id) == 0) {
            if (kill(containers[i].pid, SIGTERM) == -1) {
                perror("kill");
                return 1;
            }
            
            if (waitpid(containers[i].pid, NULL, 0) == -1) {
                perror("waitpid");
                return 1;
            }
            
            // Remove container from the list
            for (int j = i; j < container_count - 1; j++) {
                containers[j] = containers[j + 1];
            }
            container_count--;
            
            printf("Container stopped: %s\n", id);
            return 0;
        }
    }
    
    fprintf(stderr, "Error: Container not found\n");
    return 1;
}