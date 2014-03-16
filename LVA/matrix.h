#ifndef _MATRIX_H_
#define _MATRIX_H_

#define N_ROWS 7
#define N_COLS 5

#define N_LEVELS 4

typedef struct matrix_t {
	int **values;
	int n_rows;
	int n_cols;
} matrix_t;

matrix_t *setup_matrix(int n_rows, int n_cols);
void reset_matrix(matrix_t *matrix);

#endif