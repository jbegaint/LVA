#include <stdio.h>
#include <stdlib.h>

#include "../include/traitement.h"

/* ***************** *
 * Taille de l'image *
 * ***************** */
#define X_PIXELS 320
#define Y_PIXELS 240


/* **************** *
 * Nombre de diodes *
 * **************** */
#define Dpc 7			// diodes par colonne
#define Dpl 5			// diodes par ligne


/* **************** *
 * Main pour tester *
 * **************** */

int main(int argc, char **argv)
{
	int i;
	FILE *fichier = NULL;

	/* Test sur la commande */
	if (argc != 3) {
		fprintf(stderr, "Usage error\n");
		return EXIT_FAILURE;
	}

	/* Allocations des matrices */
	int **Depth = (int **) malloc(Y_PIXELS * sizeof(int *));
	for (i = 0; i < X_PIXELS; i++)
		Depth[i] = (int *) malloc(X_PIXELS * sizeof(int));


	int **Image = (int **) malloc(Dpc * sizeof(int *));
	for (i = 0; i < Dpc; i++)
		Image[i] = (int *) malloc(Dpl * sizeof(int));


	/* Test d'ouverture du fichier */
	fichier = fopen(argv[1], "r");
	if (fichier == NULL) {
		fprintf(stdout, "Cannot open the file\n");
		exit(1);
	}


	/* Remplissage de la matrice avec les coefficients du fichier */
	RempliMatriceReelle(Depth, fichier);
	Depth = centring(Depth, X_PIXELS, Y_PIXELS);

	/* Moyennage des coefficients */
	Image = MedianDepth(Depth);
	Image = centring(Image, Dpl, Dpc);
	Image = thresholding(Image, Dpl, Dpc);

	/* Creation du fichier PGM */
	writePGM_5x7(argv[2], Image);

	return EXIT_SUCCESS;
}
