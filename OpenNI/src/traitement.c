#include <stdio.h>
#include <stdlib.h>

#include "traitement.h"

/* ***************** *
 * Taille de l'image *
 * ***************** */
#define X_PIXELS 320
#define Y_PIXELS 240

/* **************** *
 * Nombre de diodes *
 * **************** */
#define Dpc 7  // diodes par colonne
#define Dpl 5  // diodes par ligne


void RempliMatriceReelle(int **Mat, FILE *pf)
{
	int i, j;

	for(i = 0; i < Y_PIXELS; i++)
		for (j = 0; j < X_PIXELS; j++)
			fscanf(pf, "%d", Mat[i] + j);
}



int** MedianDepth(int** depth)
{	
	int i, j;
	int dX = Y_PIXELS / Dpc; 
	int dY = X_PIXELS / Dpl; 

	int** Tab=(int **)malloc(Dpc * sizeof(int*));
	for(i = 0; i < Dpc; i++)
		Tab[i]=(int *)malloc(Dpl * sizeof(int));

	/* Seuillage des coefficients */
	for(i = 0; i < Dpc; i++)
		for(j = 0; j < Dpl; j++)
			Tab[i][j]=(int)Moyenne(depth, i * dX, j * dY, dX, dY);
 
	return Tab;
}


float Moyenne(int** depth, int _x, int _y, int dx, int dy)
{
	int i = 0, j = 0;
	float moy = 0;

	for(i = 0; i < dx; i++)
		for(j = 0; j < dy; j++)
			moy = moy + depth[_x + i][_y + j];	

	return (moy / (dx * dy));
}


void writePGM_5x7(const char *filename, int ** data)
{
	FILE *pgmFile;
	int i, j, level;

	/* affichage des valeurs des coefficients dans le terminal */
	for (i = 0; i < Dpc; i++)
	{
		for (j = 0; j < Dpl; j++) 
		{  
			level = data[i][j];
			fprintf(stdout, "%d\t", level);
		}

		printf("\n");
	}

	/* Ouverture du fichier */
	pgmFile = fopen(filename, "wb");
	
	if (pgmFile == NULL)
	{
		printf("Cannot open file to write");
		exit(EXIT_FAILURE);
	}

	/* En-tete du fichier PGM */
	fprintf(pgmFile, "P2\n#\n#\n");
	fprintf(pgmFile, "%d %d\n", Dpl, Dpc);
	fprintf(pgmFile, "255\n");

	/* Ecriture des coefficients */
	for (i = 0 ; i < Dpc; i++)
		for (j = 0 ; j < Dpl; j++) 
			fprintf(pgmFile, "%d ", data[i][j]);
	
	fclose(pgmFile);
}


/* Recentre les valeurs entre 0 et 255 */
int** centrage( int** depth, int X, int Y)
{	
	int i, j, max = 0;

	int** Tab = (int **)malloc(Y * sizeof(int*));	
	for (i = 0; i < Y; i++)
		Tab[i]=(int *)malloc(X * sizeof(int));

	/* recherche du max */
	for (i = 0 ; i < Y; i++)
		for (j = 0 ; j < X; j++) 
			if(depth[i][j] > max)
				max = depth[i][j];

	/* centrage des coefficients */
	for (i = 0 ; i < Y; i++)
		for (j = 0 ; j < X; j++)
			Tab[i][j] = (int)(255 * depth[i][j] / max);

	return Tab;
}


int** seuillage(int** depth, int X, int Y)
{	
	int i, j, max = 0;

	int** Tab = (int **)malloc(Y * sizeof(int*));
	for (i = 0; i < Y; i++)
		Tab[i]=(int *)malloc(X * sizeof(int));

	/* recherche du max */
	for (i = 0 ; i < Y; i++)
		for (j = 0 ; j < X; j++)
			if(depth[i][j] > max)
				max = depth[i][j];

	/* seuillage des coefficients */
	for (i = 0 ; i < Y; i++)
		for (j = 0 ; j < X; j++) 
		{  
			Tab[i][j]=(int)(255 * depth[i][j] / max);
		
			if (Tab[i][j] > 191)
				Tab[i][j] = 255;
			else if (Tab[i][j] > 127)
				Tab[i][j] = 181;
			else if (Tab[i][j] > 63)
				Tab[i][j] = 63;
			else 
				Tab[i][j]=0;
		}

	return Tab;
}
