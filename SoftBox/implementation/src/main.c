#include "main.h"

void print_usage(void) {
    printf("Usage: softbox <command> [args...]\n");
    printf("Commands:\n");
    printf("  run <image>     Run a new container\n");
    printf("  list            List running containers\n");
    printf("  stop <id>       Stop a running container\n");
    printf("  images          List available images\n");
    printf("  pull <image>    Pull a new image\n");
}

int handle_run_command(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Error: Missing image name\n");
        return 1;
    }
    char *image = argv[2];
    return run_container(image);
}

int handle_list_command(void) {
    return list_containers();
}

int handle_stop_command(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Error: Missing container ID\n");
        return 1;
    }
    char *id = argv[2];
    return stop_container(id);
}

int handle_images_command(void) {
    return list_images();
}

int handle_pull_command(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Error: Missing image name\n");
        return 1;
    }
    char *image = argv[2];
    return pull_image(image);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    if (setup_image_store() == -1) {
        fprintf(stderr, "Failed to set up image store\n");
        return 1;
    }

    char *command = argv[1];

    if (strcmp(command, "run") == 0) {
        return handle_run_command(argc, argv);
    } else if (strcmp(command, "list") == 0) {
        return handle_list_command();
    } else if (strcmp(command, "stop") == 0) {
        return handle_stop_command(argc, argv);
    } else if (strcmp(command, "images") == 0) {
        return handle_images_command();
    } else if (strcmp(command, "pull") == 0) {
        return handle_pull_command(argc, argv);
    } else {
        printf("Unknown command: %s\n", command);
        print_usage();
        return 1;
    }
}