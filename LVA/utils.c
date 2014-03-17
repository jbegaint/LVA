#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "matrix.h"

void die(char *err, ...)
{
	va_list e;

	va_start(e, err);
	vfprintf(stderr, err, e);
	va_end(e);

	exit(EXIT_FAILURE);
}

void print_bits(int val)
{
	for (int i = 31; i >= 0; --i)
		fprintf(stdout, "%d", (val >> i) & 1);

	fprintf(stdout, "\n");
}

void writePGM_5x7(const char *filename, int **data)
{
	FILE *pgmFile;
	int i, j, level;

	/* affichage des valeurs des coefficients dans le terminal */
	for (i = 0; i < N_ROWS; i++) {
		for (j = 0; j < N_COLS; j++) {
			level = data[i][j];
			fprintf(stdout, "%d\t", level);
		}

		printf("\n");
	}

	/* Ouverture du fichier */
	pgmFile = fopen(filename, "wb");

	if (pgmFile == NULL) {
		printf("Cannot open file to write");
		exit(EXIT_FAILURE);
	}

	/* En-tete du fichier PGM */
	fprintf(pgmFile, "P2\n#\n#\n");
	fprintf(pgmFile, "%d %d\n", N_COLS, N_ROWS);
	fprintf(pgmFile, "3\n");

	/* Ecriture des coefficients */
	for (i = 0; i < N_ROWS; i++)
		for (j = 0; j < N_COLS; j++)
			fprintf(pgmFile, "%d ", data[i][j]);

	fclose(pgmFile);
}