#include "linalg/matrix.c"

double invert(double x) {
    return 1.0 / x;
}

int main() {
    Matrix *m = matrix_create(4, 3);
    printf("Created\n");
    matrix_print(m);
    printf("Printed\n");
    matrix_fill(m, 5.0);
    printf("Filled\n");
    matrix_print(m);
    printf("Printed\n");

    m->entries[0][2] = 20.0;
    printf("Modified\n");
    matrix_print(m);
    printf("Printed\n");

    Matrix * m1 = matrix_transpose(m);
    printf("Transposed\n");
    matrix_print(m1);
    printf("Printed\n");

    Matrix *m2 = dot(m, m1);
    printf("Matrix Multiplied\n");
    matrix_print(m2);
    printf("Printed\n");

    Matrix *m3 = matrix_flatten(m, 0);
    printf("Flattened (Axis=0)\n");
    matrix_print(m3);
    printf("Printed\n");
    int mxidx = matrix_argmax(m3);
    printf("Argmax: %d\n", mxidx);

    Matrix *m4 = matrix_flatten(m, 1);
    printf("Flattened (Axis=1)\n");
    matrix_print(m4);
    printf("Printed\n");

    Matrix *m5 = apply(invert, m);
    printf("Inverted\n");
    matrix_print(m5);
    printf("Printed\n");

    Matrix *m6 = matrix_reshape(m, 2, 6);
    printf("Reshaped\n");
    matrix_print(m6);
    printf("Printed\n");

    matrix_free(m);
    matrix_free(m2);
    matrix_free(m3);
    matrix_free(m4);
    printf("Freed\n");
    return 0;
}