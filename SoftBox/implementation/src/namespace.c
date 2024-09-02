#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "namespace.h"

int setup_container_namespaces() {
    if (unshare(CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWNS | CLONE_NEWNET) == -1) {
        perror("unshare");
        return -1;
    }
    return 0;
}