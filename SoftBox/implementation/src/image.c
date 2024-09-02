#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "image.h"

#define IMAGE_DIR "/var/lib/softbox/images"
#define MAX_CMD_LENGTH 1024
#define MAX_PATH_LENGTH 256

int setup_image_store() {
    struct stat st = {0};
    if (stat(IMAGE_DIR, &st) == -1) {
        if (mkdir(IMAGE_DIR, 0700) == -1) {
            perror("Failed to create image directory");
            return -1;
        }
    }
    return 0;
}

int pull_image(const char* image_name) {
    char cmd[MAX_CMD_LENGTH];
    
    // Pull the image using Docker
    snprintf(cmd, sizeof(cmd), "docker pull %s", image_name);
    if (system(cmd) != 0) {
        fprintf(stderr, "Failed to pull image: %s\n", image_name);
        return -1;
    }
    
    // Create a directory for the image
    char image_dir[MAX_PATH_LENGTH];
    snprintf(image_dir, sizeof(image_dir), "%s/%s", IMAGE_DIR, image_name);
    if (mkdir(image_dir, 0700) == -1) {
        perror("Failed to create image directory");
        return -1;
    }
    
    // Save the image to our custom directory
    snprintf(cmd, sizeof(cmd), "docker save %s | tar -x -C %s", image_name, image_dir);
    if (system(cmd) != 0) {
        fprintf(stderr, "Failed to save image: %s\n", image_name);
        return -1;
    }
    
    printf("Image pulled successfully: %s\n", image_name);
    return 0;
}

int list_images() {
    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir(IMAGE_DIR)) != NULL) {
        printf("AVAILABLE IMAGES:\n");
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                printf("%s\n", ent->d_name);
            }
        }
        closedir(dir);
    } else {
        perror("Failed to open image directory");
        return -1;
    }
    
    return 0;
}

char* get_image_path(const char* image_name) {
    static char path[MAX_PATH_LENGTH];
    snprintf(path, sizeof(path), "%s/%s", IMAGE_DIR, image_name);
    return path;
}