#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "filesystem.h"
#include "image.h"

#define MAX_PATH 1024

int setup_container_fs(const char* image) {
    char buf[MAX_PATH];
    char* rootfs = get_image_path(image);

    // Change root
    if (chroot(rootfs) == -1) {
        perror("chroot");
        return -1;
    }

    // Change directory to new root
    if (chdir("/") == -1) {
        perror("chdir");
        return -1;
    }

    // Mount proc filesystem
    if (mount("proc", "/proc", "proc", 0, NULL) == -1) {
        perror("mount proc");
        return -1;
    }

    return 0;
}