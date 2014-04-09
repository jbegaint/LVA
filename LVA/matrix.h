#ifndef _MATRIX_H_
#define _MATRIX_H_

#define PIXELS_X 320
#define PIXELS_Y 240

#define N_ROWS 7
#define N_COLS 5
#define N_GPIOS 4

#define N_LEVELS 4

typedef struct matrix_t {
	int **values;
	int n_rows;
	int n_cols;
} matrix_t;

matrix_t *init_matrix(int n_rows, int n_cols);
matrix_t *get_led_matrix(matrix_t *img_matrix);
matrix_t *get_resized_matrix(matrix_t *matrix, int n_rows, int n_cols);
matrix_t *get_cropped_matrix(matrix_t *m, int xmin, int ymin, int xmax, int ymax);

void reset_matrix(matrix_t *matrix);
void threshold_matrix(matrix_t *matrix);
void center_matrix(matrix_t *matrix);
void free_matrix(matrix_t *matrix);
void copy_matrix(matrix_t *dest, matrix_t *src);
void matrix_toggle_pin(matrix_t *m, int i, int j);

int get_matrix_max(matrix_t *matrix);
int get_matrix_min(matrix_t *matrix);

float Moyenne(int **depth, int _x, int _y, int dx, int dy);

void print_matrix_infos(matrix_t *matrix);
void print_matrix(matrix_t *matrix);
#endif
