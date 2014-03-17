#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "pgm_utils.h"
#include "matrix.h"


/* Credits: Phelma 1st Year nano-projet*/


matrix_t *read_pgm_file(char *fileName)
{
	FILE *filein;
	filein = fopen(fileName, "rb");

	int n_cols, n_rows;

	if (filein == NULL) {
		fprintf(stderr,
			"Erreur: impossible d'ouvrir le fichier '%s'\n",
			fileName);
	}

	char line[256];

	/*lecture de la première ligne */
	if (fgets(line, sizeof(line), filein) != NULL) {
		if (strcmp(line, "P5\n") != 0) {
			fprintf(stderr, "Erreur pas un fichier P5");
			return NULL;
		}
	}

	/*affichage du commentaire */
	if (fgets(line, sizeof(line), filein) != NULL) {
		printf("Commmentaire: %s", line);
	}

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

	/*on vérifie si la valeur max dépasse pas 255 */
	if (fgets(line, sizeof(line), filein) != NULL) {
		int len = atoi(line);
		if (len > 255) {
			fprintf(stderr,
				"Erreur: sur la valeur max d'un pixel\n");
		}
	} else {
		return NULL;
	}

	matrix_t *img;
	img = init_matrix(n_rows, n_cols);


	int nr;
	nr = fread(*(img->values), sizeof(unsigned char),
		   img->n_cols * img->n_rows, filein);
	if (nr != img->n_cols * img->n_rows) {
		fprintf(stderr, "Erreur : erreur de lecture du fichier\n");
		return NULL;
	}

	fclose(filein);
	return img;
}