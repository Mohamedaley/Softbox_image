#ifndef IMAGE_H
#define IMAGE_H

int setup_image_store(void);
int pull_image(const char* image_name);
int list_images(void);
char* get_image_path(const char* image_name);

#endif // IMAGE_H