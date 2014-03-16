#ifndef _MATRIX_H_
#define _MATRIX_H_

#define N_ROWS 7
#define N_COLS 5

#define PIXELS_X 320
#define PIXELS_Y 240

#define N_LEVELS 4

typedef struct matrix_t {
	int **values;
	int n_rows;
	int n_cols;
} matrix_t;

matrix_t *init_matrix(int n_rows, int n_cols);
matrix_t *get_led_matrix(matrix_t *img_matrix);
matrix_t *get_resized_matrix(matrix_t *matrix, int n_rows, int n_cols);

void reset_matrix(matrix_t *matrix);
void threshold_matrix(matrix_t *matrix);
void center_matrix(matrix_t *matrix);

int get_matrix_max(matrix_t *matrix);

float Moyenne(int **depth, int _x, int _y, int dx, int dy);

#endif