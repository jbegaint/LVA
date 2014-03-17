#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include "matrix.h"
#include "patterns.h"
#include "pgm_utils.h"
#include "utils.h"

int next_frame = 0;

static char *img_name;

void write_wrapper(char *filename, matrix_t *matrix)
{
	char *tmp = calloc(256, sizeof(char));

	strcat(tmp, img_name);
	strcat(tmp, "_");
	strcat(tmp, filename);
	printf("%s\n", tmp); 
	write_pgm_file(tmp, matrix, "Generated by pgm_test");
}

int main(int argc, char **argv)
{
	matrix_t *img;
	matrix_t *grid;

	if (argc != 2) {
		die("usage: %s file.pgm\n", argv[0]);
	}

	img_name = strdup(argv[1]);

	img = read_pgm_file(img_name);
	print_matrix(img);
	getchar();
	
	center_matrix(img);
	write_wrapper("img_center.pgm", img);
	print_matrix(img);
	getchar();

	grid = get_resized_matrix(img, N_ROWS, N_COLS);
	write_wrapper("grid_resized.pgm", grid);
	print_matrix(grid);
	getchar();

	center_matrix(grid);
	write_wrapper("grid_centered.pgm", grid);
	print_matrix(grid);
	getchar();

	threshold_matrix(grid);
	write_wrapper("grid_threshold.pgm", grid);
	print_matrix(grid);
	getchar();

	return 0;
}