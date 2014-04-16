#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
		for (int j = 0; j < matrix->n_cols; ++j) {
			(matrix->values)[i][j] = 3;
		}		
	}
}

/**
 * @brief Free a matrix structure
 * 
 * @param matrix the matrix to be freed
 */
void free_matrix(matrix_t *matrix)
{
	free(*(matrix->values));
	free(matrix->values);
	free(matrix);
}

/**
 * @brief Copy a matrix structure
 * 
 * @param src the source matrix
 * @param dest the destination matrix 
 */
void copy_matrix(matrix_t *dest, matrix_t *src)
{
	dest->n_rows = src->n_rows;
	dest->n_cols = src->n_cols;

	memcpy(*(dest->values), *(src->values), src->n_cols * src->n_rows * sizeof(**(dest->values)));
}

/**
 * @brief Get a matrix structure for the led grid (resized, centre, threshold)
 * 
 * @param img_matrix the source matrix, extracted from a frame
 * 
 * @return a matrix structure for the led grid
 */
matrix_t *get_led_matrix(matrix_t *img_matrix)
{
	matrix_t *led_matrix;

	/* centre initial matrix */
	center_matrix(img_matrix);

	led_matrix = get_resized_matrix(img_matrix, N_ROWS, N_COLS);
	center_matrix(led_matrix);
	threshold_matrix(led_matrix);

	return led_matrix;
}

matrix_t *get_cropped_matrix(matrix_t *m, int xmin, int ymin, int xmax, int 
		ymax)
{
	matrix_t *res = init_matrix(ymax, xmax);

	for (int i = ymin; i < ymax; ++i) {
		for (int j =  xmin; j < xmax; ++j) {
			(res->values)[i-ymin][j-xmin] = (m->values)[i][j];
		}
	}	

	return res;
}

matrix_t *get_resized_matrix(matrix_t *matrix, int n_rows, int n_cols)
{
	matrix_t *new_matrix;
	int dX = (matrix->n_rows) / n_rows;
	int dY = (matrix->n_cols) / n_cols;

	new_matrix = init_matrix(n_rows, n_cols);

	/* Seuillage des coefficients */
	for (int i = 0; i < n_rows; ++i) {
		for (int j =  0; j < n_cols; ++j) {
			/* (new_matrix->values)[i][j] = (int) Moyenne(matrix->values, i * dX, j 
					* dY, dX, dY); */
			(new_matrix->values)[i][j] = (matrix->values)[i * dX + dX/2][j * dY + dY/2];
		}
	}	

	return new_matrix;
}

/* Recentre les valeurs entre 0 et 255 */
void center_matrix(matrix_t *matrix)
{
	int max = get_matrix_max(matrix);
	int min = get_matrix_min(matrix);

	/* centrage des coefficients */
	for (int i = 0; i < matrix->n_rows; i++) {
		for (int j = 0; j < matrix->n_cols; j++) {
			(matrix->values)[i][j] = (int) (255 * ((matrix->values)[i][j] - min) 
					/ (max - min + 1 )) ;
		}
	}
}

/* Seuillage des coefficients en 4 niveaux */
void threshold_matrix(matrix_t *matrix)
{
	/* seuillage des coefficients */
	for (int i = 0; i < matrix->n_rows; i++) {
		for (int j = 0; j < matrix->n_cols; j++) {

			if ((matrix->values)[i][j] > 100)
				(matrix->values)[i][j] = 3;
			else if ((matrix->values)[i][j] > 60)
				(matrix->values)[i][j] = 2;
			else if ((matrix->values)[i][j] > 20)
				(matrix->values)[i][j] = 1;
			else
				(matrix->values)[i][j] = 0;
		}
	}
}

void center_and_threshold_matrix(matrix_t *matrix)
{
	int max, min, val, tmp;

	max = get_matrix_max(matrix);
	min = get_matrix_min(matrix);

	for (int i = 0; i < matrix->n_rows; ++i) {
		for (int j = 0; j < matrix->n_cols; ++j) {
			val  = (matrix->values)[i][j];
			tmp = (255 * val) / (max - min + 1);

			if (tmp > 192)
				(matrix->values)[i][j] = 3;
			else if (tmp > 128)
				(matrix->values)[i][j] = 2;
			else if (tmp > 64)
				(matrix->values)[i][j] = 1;
			else 
				(matrix->values)[i][j] = 0;
		}
	}
}


/*
 * @brief Get the minimal value in a matrix structure
 *
 * @param matrix the matrix
 *
 * @return the min
 */
int get_matrix_min(matrix_t *matrix)
{
	int min = (matrix->values)[0][0] ;

	for (int i = 0; i < matrix->n_rows; ++i) {
		for (int j = 0; j < matrix->n_cols; ++j) {
			if ((matrix->values)[i][j] < min) {
				min = (matrix->values)[i][j];
			}
		}
	}

	return min;
}

/*
 * @brief Get the maximum value in a matrix structure
 *
 * @param matrix the matrix
 *
 * @return the max
 */
int get_matrix_max(matrix_t *matrix)
{
	int max = (matrix->values)[0][0];

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

	for (i = 0; i < dx; i++) {
		for (j = 0; j < dy; j++) {
			moy = moy + depth[_x + i][_y + j];
		}
	}

	return (moy / (dx * dy));
}

void print_matrix_infos(matrix_t *matrix)
{
	printf("n_rows: %d, n_cols: %d", matrix->n_rows, matrix->n_cols);
}

void print_matrix(matrix_t *matrix)
{
	for (int i = 0; i < matrix->n_rows; ++i) {
		for (int j = 0; j < matrix->n_cols; ++j) {
			printf("%d ", (matrix->values)[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void matrix_toggle_pin(matrix_t *m, int i, int j)
{
	if ((m->values)[i][j] == 0) {
		(m->values)[i][j] = 3;
	}
	else {
		(m->values)[i][j] = 0;
	}
}
