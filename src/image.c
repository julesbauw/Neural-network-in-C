#include "image.h"
#include <stdlib.h>
#include <stdio.h>  
#include <string.h>
#define MAX_CHAR 10000


Image* read_image_from_line(char* line) {

    Image* im = malloc(sizeof(Image));

    if (!im) {
        return NULL;
    }

    char* token = strtok(line,",");
    int j = 0;
    im->input = create_matrix(IMAGE_X * IMAGE_Y,1);
    im->label = create_matrix(OUTPUT_AMOUNT,1);
    while(token)
    {  
        if (j == 0) {
            int label = atoi(token);

            matrix_set(im->label,label,0,1.0);
        }
        else {
            int value = atoi(token);
            double v = ((double) value) / 256.0;
            matrix_set(im->input,(j - 1),0,v);
        }
        token = strtok(NULL,",");
        j++;
    }
    
    return im;
}

LinkedList* read_images_from_file(char* file_name,int amount) {
    /*
    Returns a linked list of images with size amount, the images are read from the file file_name (the first amount images)
    */
    FILE* fptr = fopen(file_name,"r");
    if (!fptr) return NULL;

    LinkedList* list = create_linked_list();

    if (!list) {
        return NULL;
    }

    printf("Reading %s\n",file_name);

    char buffer[MAX_CHAR];

    int is_end = 0;
    int j = 0;
    
    while(!is_end && j <= amount) {
        char * succes = fgets(buffer,MAX_CHAR,fptr);

        if (!succes) {
            is_end = 1;
            continue;
        }

        if (j == 0) {
            // we don't read the labels
            j++;
            continue;
        }
        Image* image = read_image_from_line(buffer);
        
        if (image) {
            linked_list_add(list,image);
        }

        j++;
    }

    fclose(fptr);

    return list;
}

void free_image(Image* im) {
    if (!im) {
        return;
    }

    free_matrix(im->input);
    free_matrix(im->label);

    im->label = NULL;
    im->input = NULL;
    free(im);
    im = NULL;
}

void print_image(Image* image) {
    matrix_print(image->label);

    for (size_t y = 0; y < IMAGE_Y; y++)
    {
        for (size_t x = 0; x < IMAGE_X; x++)
        {
            double val = matrix_get(image->input,IMAGE_Y * y + x,0);
            if (val > 0.0) {
                printf("1");
            }
            else {
                printf("0");
            }
        }
        printf("\n");
    }
    
}