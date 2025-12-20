#ifndef IMAGE

#define IMAGE
#define IMAGE_X 28
#define IMAGE_Y 28
#define OUTPUT_AMOUNT 10

#include "matrix.h"
#include "list.h"

typedef struct {
    Matrix* input;
    Matrix* label; //label
} Image;


void free_image(Image* im);

Image* read_image_from_line(char* line);

LinkedList* read_images_from_file(char* file_name,int amount);

void print_image(Image* image);


#endif