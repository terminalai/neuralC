#include "data.h"
#include "../linalg/matrix.h"

Entry *entry_create(int rows, int cols) {
    Entry *entry = malloc(sizeof(Entry));
    entry->mat = matrix_create(rows, cols);
    entry->label = 0;
}