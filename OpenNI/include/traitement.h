#ifndef _TRAITEMENT_H_
#define _TRAITEMENT_H_

float Moyenne(int **depth, int _x, int _y, int dx, int dy);
int **MedianDepth(int **depth);
int **centring(int **depth, int X, int Y);
int **thresholding(int **depth, int X, int Y);
void RempliMatriceReelle(int **Mat, FILE * pf);

#endif
