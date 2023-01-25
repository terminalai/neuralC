//
// Created by Prannaya on 23/5/2022.
//

#include "matrix.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 100

Matrix* matrix_create(int row, int col) {
    Matrix *matrix = malloc(sizeof(Matrix));
    matrix->rows = row;
    matrix->cols = col;
    matrix->entries = malloc(row * sizeof(double*));
    for(int i = 0; i < row; i++) 
        matrix->entries[i] = malloc(col * sizeof(double));
    return matrix;
}

void matrix_fill(Matrix *m, double n) {
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            m->entries[i][j] = n;
        }
    }
}

void matrix_free(Matrix *m) {
    for(int i = 0; i < m->rows; i++) free(m->entries[i]);
    free(m);
    m = NULL;
}

void matrix_print(Matrix* m) {
    printf("Rows: %d Columns %d\n", m->rows, m->cols);
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            printf("%1.3f\t", m->entries[i][j]);
        }
        printf("\n");
    }
}

Matrix* matrix_copy(Matrix *m) {
    Matrix* mat = matrix_create(m->rows, m->cols);
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            mat->entries[i][j] = m->entries[i][j];
        }
    }
    return mat;
}

void matrix_save(Matrix* m, char* file_string) {
    FILE* file = fopen(file_string, "w");
    fprintf(file, "%d\n%d\n", m->rows, m->cols);
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            fprintf(file, "%.6f\n", m->entries[i][j]);
        }
    }
    printf("Successfully saved matrix to %s\n", file_string);
    fclose(file);
}

Matrix* matrix_load(char* file_string) {
    FILE* file = fopen(file_string, "r");
    char entry[MAXCHAR];
    fgets(entry, MAXCHAR, file);
    int rows = atoi(entry);
    fgets(entry, MAXCHAR, file);
    int cols = atoi(entry);
    Matrix *m = matrix_create(rows, cols);
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            fgets(entry, MAXCHAR, file);
            m->entries[i][j] = strtod(entry, NULL);
        }
    }
    printf("Successfully loaded matrix from %s\n", file_string);
    fclose(file);
    return m;
}

int matrix_argmax(Matrix *m) {
    // Expects a Mx1 matrix
    double max_score = 0;
    int max_idx = 0;
    for(int i = 0; i < m->rows; i++) {
        if (m->entries[i][0] > max_score) {
            max_score = m->entries[i][0];
            max_idx = i;
        }
    }
    return max_idx;
}

Matrix* matrix_transpose(Matrix *m) {
    Matrix* mat = matrix_create(m->cols, m->rows);
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            mat->entries[j][i] = m->entries[i][j];
        }
    }
    return mat;
}

Matrix* matrix_flatten(Matrix* m, int axis) {
    // Axis = 0 -> Column Vector, Axis = 1 -> Row Vector
    Matrix* mat = matrix_create(m->rows * m->cols, 1);
    if(axis != 0 && axis != 1) {
        printf("Argument to matrix_flatten must be 0 or 1");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            if(axis == 1) {
                mat->entries[i * m->cols + j][0] = m->entries[i][j];
            } else {
                mat->entries[j * m->cols + i][0] = m->entries[i][j];
            }
            // mat->entries[j * ((m->rows-1)*axis + 1) + (m->cols-1)*(1-axis) + 1][0] = m->entries[i][j];
        }
    }
    if(axis == 1) {
        Matrix* new_mat = matrix_transpose(mat);
        matrix_free(mat);
        return new_mat;
    }
    return mat;
}


Matrix* multiply(Matrix* m1, Matrix* m2) {
    if(m1->rows != m2->rows || m1->cols != m2->cols) {
        printf("Matrices should be of the same size (multiply)\n");
        exit(EXIT_FAILURE);
    }
    Matrix* m = matrix_create(m1->rows, m1->cols);
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            m->entries[i][j] = m1->entries[i][j] * m2->entries[i][j];
        }
    }
    return m;
}

Matrix* add(Matrix* m1, Matrix* m2) {
    if(m1->rows != m2->rows || m1->cols != m2->cols) {
        printf("Matrices should be of the same size (add)\n");
        exit(EXIT_FAILURE);
    }
    Matrix* m = matrix_create(m1->rows, m1->cols);
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            m->entries[i][j] = m1->entries[i][j] + m2->entries[i][j];
        }
    }
    return m;
}

Matrix* subtract(Matrix* m1, Matrix* m2) {
    if(m1->rows != m2->rows || m1->cols != m2->cols) {
        printf("Matrices should be of the same size (subtract)\n");
        exit(EXIT_FAILURE);
    }
    Matrix* m = matrix_create(m1->rows, m1->cols);
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            m->entries[i][j] = m1->entries[i][j] - m2->entries[i][j];
        }
    }
    return m;
}

Matrix* dot(Matrix* m1, Matrix* m2) {
    if(m1->cols != m2->rows) {
        printf("Matrices should be of format l x m and m x n");
        exit(EXIT_FAILURE);
    }
    Matrix* m = matrix_create(m1->rows, m2->cols);
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            int sum = 0;
            for(int k = 0; k < m1->cols; k++) {
                sum += m1->entries[i][k] * m2->entries[k][j];
            }
            m->entries[i][j] = sum;
        }
    }
    return m;
}

Matrix* apply(double (*func)(double), Matrix* m) {
    Matrix* mat = matrix_create(m->rows, m->cols);
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            mat->entries[i][j] = (*func)(m->entries[i][j]);
        }
    }
    return mat;
}

Matrix* scale(double n, Matrix* m) {
    Matrix* mat = matrix_copy(m);
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            mat->entries[i][j] *= n;
        }
    }
    return mat;
}

Matrix* addScalar(double n, Matrix* m) {
    Matrix* mat = matrix_copy(m);
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            mat->entries[i][j] += n;
        }
    }
    return mat;
}


double sum(Matrix* m) {
    double total = 0;
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            total += m->entries[i][j];
        }
    }
    return total;
}
