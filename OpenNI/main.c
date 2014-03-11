#include <stdio.h>
#include <stdlib.h>
#include "traitement.h"

/*********************
 * Taille de l'image *
 * *******************/
#define X_PIXELS 320
#define Y_PIXELS 240

/**********************
 * nombre de diodes ****
 * *********************/
#define Dpc 14  // diodes par colonne
#define Dpl 25  // diodes par ligne



/************************************
* ***** Main pour tester ***********
* *********************************/

int main ( int argc, char** argv)
{
  int i;
  FILE *fichier = NULL;
 
 /* Allocations des matrices */
  int** Depth = (int **)malloc(Y_PIXELS*sizeof(int*));
  for(i=0;i<X_PIXELS;i++)
  {  
    Depth[i]=(int *)malloc(X_PIXELS*sizeof(int));
  }
  
  int** Image=(int **)malloc(Dpc*sizeof(int*));
  for( i = 0 ; i < Dpc ; i++ )
  {  
    Image[i] = (int *)malloc(Dpl*sizeof(int));
  }
 
 //test d'ouverture du fichier
 fichier = fopen(argv[1], "r"); // ouvrir en lecture
 if(fichier == NULL){
      printf("Impossible d'ouvrir le fichier");
      exit(1);
  }

  /* Remplissage de la matrice avec les coefficients du fichier */
  RempliMatriceReelle(Depth,fichier);
  Depth=centrage(Depth,X_PIXELS,Y_PIXELS);  
  
  /* Moyennage des coefficients */
  Image = MedianDepth(Depth);
  Image = centrage(Image, Dpl, Dpc);
  Image = seuillage(Image, Dpl, Dpc);

  /* Creation des fichiers PGM */
  writePGM_25x14(argv[2], Image);
  writePGM_320x240(argv[3], Depth);


      
  return EXIT_SUCCESS;
}

