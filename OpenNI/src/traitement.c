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


void RempliMatriceReelle(int **Mat, FILE *pf)
{
	int i, j;

	for (i = 0; i < Y_PIXELS; i++)
		for (j = 0; j < X_PIXELS; j++)
			fscanf(pf, "%d", Mat[i] + j);
}


int **MedianDepth(int **depth)
{
	int i, j;
	int dX = Y_PIXELS / Dpc;
	int dY = X_PIXELS / Dpl;

	int **Tab = (int **) malloc(Dpc * sizeof(int *));
	for (i = 0; i < Dpc; i++)
		Tab[i] = (int *) malloc(Dpl * sizeof(int));

	/* Seuillage des coefficients */
	for (i = 0; i < Dpc; i++)
		for (j = 0; j < Dpl; j++)
			Tab[i][j] = (int) Moyenne(depth, i * dX, j * dY, dX, dY);

	return Tab;
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


/* Recentre les valeurs entre 0 et 255 */
int **centring(int **depth, int X, int Y)
{
	int i, j, max = 0;

	int **Tab = (int **) malloc(Y * sizeof(int *));
	for (i = 0; i < Y; i++)
		Tab[i] = (int *) malloc(X * sizeof(int));

	/* recherche du max */
	for (i = 0; i < Y; i++)
		for (j = 0; j < X; j++)
			if (depth[i][j] > max)
				max = depth[i][j];

	/* centrage des coefficients */
	for (i = 0; i < Y; i++)
		for (j = 0; j < X; j++)
			Tab[i][j] = (int) (255 * depth[i][j] / max);

	return Tab;
}

/* Seuillage des coefficients en 4 niveaux */
int **thresholding(int **depth, int X, int Y)
{
	int i, j, max = 0;

	int **Tab = (int **) malloc(Y * sizeof(int *));
	for (i = 0; i < Y; i++)
		Tab[i] = (int *) malloc(X * sizeof(int));

	/* recherche du max */
	for (i = 0; i < Y; i++)
		for (j = 0; j < X; j++)
			if (depth[i][j] > max)
				max = depth[i][j];

	/* seuillage des coefficients */
	for (i = 0; i < Y; i++)
		for (j = 0; j < X; j++) {
			Tab[i][j] = (int) (255 * depth[i][j] / max);

			if (Tab[i][j] > 191)
				Tab[i][j] = 3;
			else if (Tab[i][j] > 127)
				Tab[i][j] = 2;
			else if (Tab[i][j] > 63)
				Tab[i][j] = 1;
			else
				Tab[i][j] = 0;
		}

	return Tab;
}
