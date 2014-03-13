#ifndef __TRAITEMENT_H__
#define __TRAITEMENT_H__

void writePGM_5x7(const char *filename, int **data);
int** centring( int** depth , int X , int Y);
int ** MedianDepth( int** depth );
float Moyenne ( int** depth, int _x, int _y , int dx, int dy);
void RempliMatriceReelle(int **Mat,FILE *pf);
int** thresholding(int** depth , int X , int Y);

#endif
