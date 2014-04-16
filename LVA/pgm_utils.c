#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "pgm_utils.h"
#include "matrix.h"

/* 
	Credits: Phelma 1st Year nano-projet
	see here : http://tdinfo.phelma.grenoble-inp.fr/1Apet/td2.html
	(slighlty modified to be used with matrix structure and P2 pgm)
*/
matrix_t *read_pgm_file(char *fileName)
{
	FILE *filein;
	filein = fopen(fileName, "r");

	int n_cols, n_rows;

	if (filein == NULL) {
		fprintf(stderr,
			"Erreur: impossible d'ouvrir le fichier '%s'\n",
			fileName);
	}

	char line[256];

	/* read first 3 lines*/
	fgets(line, sizeof(line), filein);

	/*Pour récupérer n_cols et n_rows */
	if (fgets(line, sizeof(line), filein) != NULL) {

		/*calcul du nombre de chiffre de n_cols */
		int n_cols_len = 0;
		do {
			n_cols_len++;
		} while (line[n_cols_len] != ' ');

		n_cols = atoi(line);
		n_rows = atoi(line + n_cols_len);

	} else {
		return NULL;
	}

	matrix_t *img;
	img = init_matrix(n_rows, n_cols);

	int tmp;

	for (int row = 0; row < n_rows; ++row) {
		for (int col = 0; col < n_cols; ++col) {
			fscanf(filein,"%d", &tmp);
			(img->values)[row][col] = tmp;
		}
	}

	fclose(filein);
	return img;
}

int write_pgm_file(char *fileName, matrix_t *p_img, char *comment)
{
	assert(p_img && p_img->values && *(p_img->values));
	FILE *fileout;
	int nb;

	fileout = fopen(fileName, "wb");
	if (fileout == NULL) {
		/* echec : gestion d'erreur */
		fprintf(stderr,
			"ERREUR : Impossible d'ouvrir le fichier '%s' !\n",
			fileName);
		return 1;
	}
	/* ecriture de la cle identifiant les fichier PPM P5 */
	nb = fprintf(fileout, "P5\n");
	if (nb <= 0) {
		fprintf(stderr,
			"ERREUR : erreur d'ecriture fichier (cas 1)\n");
		return 1;
	}
	/* ecriture du commentaire optionnel */
	if (comment) {
		nb = fprintf(fileout, "# %s\n", comment);
		if (nb <= 0) {
			fprintf(stderr,
				"ERREUR : erreur d'ecriture fichier (cas 2)\n");
			return 1;
		}
	}
	/* nbm, n_cols et max value des pixels */
	nb = fprintf(fileout, "%d %d\n", p_img->n_cols, p_img->n_rows);
	if (nb <= 0) {
		fprintf(stderr,
			"ERREUR : erreur d'ecriture fichier (cas 3)\n");
		return 1;
	}
	nb = fprintf(fileout, "255\n");
	if (nb <= 0) {
		fprintf(stderr,
			"ERREUR : erreur d'ecriture fichier (cas 4)\n");
		return 1;
	}
	/* values : les pixels, en binaire */
	nb = fwrite(*(p_img->values), sizeof(int),
		    p_img->n_cols * p_img->n_rows, fileout);
	if (nb != p_img->n_cols * p_img->n_rows) {
		fprintf(stderr,
			"ERREUR : erreur d'ecriture fichier (cas 5)\n");
		return 1;
	}
	fclose(fileout);
	return 0;
}