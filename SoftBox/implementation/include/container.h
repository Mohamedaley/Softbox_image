#ifndef CONTAINER_H
#define CONTAINER_H

int run_container(char* image);
int list_containers(void);
int stop_container(char* id);

#endif // CONTAINER_H