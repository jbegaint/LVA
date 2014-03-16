#include <stdlib.h>

#include "matrix.h"
#include "utils.h"

/**
 * @brief Allocate a matrix structure
 *
 * @param n_rows number of rows
 * @param n_cols number of columns
 *
 * @return a pointer to the allocated matrix
 */
matrix_t *init_matrix(int n_rows, int n_cols)
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

matrix_t *get_led_matrix(matrix_t *img_matrix)
{
	matrix_t *led_matrix;

	led_matrix = get_resized_matrix(img_matrix, N_ROWS, N_COLS);
	threshold_matrix(led_matrix);
	center_matrix(led_matrix);

	return led_matrix;
}

matrix_t *get_resized_matrix(matrix_t *matrix, int n_rows, int n_cols)
{
	matrix_t *new_matrix;
	int dX = PIXELS_Y / N_ROWS;
	int dY = PIXELS_X / N_COLS;

	new_matrix = init_matrix(n_rows, n_cols);

	/* Seuillage des coefficients */
	for (int i = 0; i < n_rows; ++i) {
		for (int j = 0; j < n_cols; ++j) {
			(new_matrix->values)[i][j] = 
				(int) Moyenne(matrix->values, i * dX, j * dY, dX, dY);
		}
	}	

	return new_matrix;
}

/* Recentre les valeurs entre 0 et 255 */
void center_matrix(matrix_t *matrix)
{
	int max = get_matrix_max(matrix);
	int **depth = matrix->values;

	/* centrage des coefficients */
	for (int i = 0; i < matrix->n_rows; i++) {
		for (int j = 0; j < matrix->n_cols; j++) {
			depth[i][j] = (int) (255 * depth[i][j] / max);
		}
	}
}

/* Seuillage des coefficients en 4 niveaux */
void threshold_matrix(matrix_t *matrix)
{
	int max = get_matrix_max(matrix);
	int **depth = matrix->values;

	/* seuillage des coefficients */
	for (int i = 0; i < matrix->n_rows; i++) {
		for (int j = 0; j < matrix->n_cols; j++) {
			depth[i][j] = (int) (255 * depth[i][j] / max);

			if (depth[i][j] > 191)
				depth[i][j] = 3;
			else if (depth[i][j] > 127)
				depth[i][j] = 2;
			else if (depth[i][j] > 63)
				depth[i][j] = 1;
			else
				depth[i][j] = 0;
		}
	}
}


/*
 * @brief Get the maximum value in the matrix
 *
 * @param matrix the matrix
 *
 * @return the max
 */
int get_matrix_max(matrix_t *matrix)
{
	int max = 0;

	for (int i = 0; i < matrix->n_rows; ++i) {
		for (int j = 0; j < matrix->n_cols; ++j) {
			if ((matrix->values)[i][j] > max) {
				max = (matrix->values)[i][j];
			}
		}
	}

	return max;
}

float Moyenne(int **depth, int _x, int _y, int dx, int dy)
{
	int i = 0, j = 0;
	float moy = 0;

	for (i = 0; i < dx; i++)
		for (j = 0; j < dy; j++)
			moy = moy + depth[_x + i][_y + j];

	return (moy / (dx * dy));
}
