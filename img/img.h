//
// Created by Prannaya on 24/5/2022.
//

#ifndef NEURALC_IMG_H
#define NEURALC_IMG_H

#include "../linalg/matrix.h"

typedef struct {
    Matrix* img_data;
    int label;
} Img;

Img** csv_to_imgs(char* file_string, int number_of_imgs);
void img_print(Img* img);
void img_free(Img* img);
void imgs_free(Img **imgs, int n);


#endif //NEURALC_IMG_H
