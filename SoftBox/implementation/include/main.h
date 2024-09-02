#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "container.h"
#include "namespace.h"
#include "cgroups.h"
#include "filesystem.h"
#include "network.h"
#include "image.h"

void print_usage(void);
int handle_run_command(int argc, char *argv[]);
int handle_list_command(void);
int handle_stop_command(int argc, char *argv[]);
int handle_images_command(void);
int handle_pull_command(int argc, char *argv[]);
int setup_container_cgroups(const char* container_id);
static char* generate_container_id();
int run_container(char* image);
int list_containers();
int stop_container(char* id);
int setup_container_fs(const char* image);
int setup_image_store();
int pull_image(const char* image_name);
int list_images();
char* get_image_path(const char* image_name);
int setup_container_namespaces();
int setup_container_network();
#endif // MAIN_H