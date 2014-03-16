#include <stdlib.h>

#include "matrix.h"

/**
 * @brief Allocate a matrix structure
 *
 * @param n_rows number of rows
 * @param n_cols number of columns
 *
 * @return a pointer to the allocated matrix
 */
matrix_t *setup_matrix(int n_rows, int n_cols)
{
	int **m;
	matrix_t *matrix;

	matrix = (matrix_t *) calloc(1, sizeof(*matrix));

	/* todo: check for alloc error */
	m = (int **) calloc(n_rows, sizeof(*m));
	*m = (int *) calloc(n_rows * n_cols, sizeof(**m));

	/* contiguous allocation */
	for (int i = 1; i < n_rows; ++i) {
		m[i] = m[i-1] + n_cols;
	}

	matrix->n_rows = n_rows;
	matrix->n_cols = n_cols;
	matrix->values = m;

	/* off value being 3, we need to set levels values to off */
	reset_matrix(matrix);

	return matrix;
}

/**
 * @brief Set the values of a matrix structure to OFF
 *
 * @param matrix the matrix to alter
 *
 */
void reset_matrix(matrix_t *matrix)
{
	for (int i = 0; i < matrix->n_rows; ++i) {
		for (int j = 0; j < matrix->n_cols ; ++j) {
			(matrix->values)[i][j] = 3;
		}		
	}
}