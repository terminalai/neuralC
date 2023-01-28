//
// Created by Prannaya on 24/5/2022.
//

#ifndef NEURALC_DATA_H
#define NEURALC_DATA_H

#include "../linalg/matrix.h"

typedef struct {
    Matrix* mat;
    int label;
} Entry;


typedef struct {
    Entry** entries;
    int rows;
    int cols;
    int entry_num;
} Dataset;


Entry* entry_create(int rows, int cols);
void fill_entry();