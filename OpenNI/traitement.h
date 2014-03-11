#ifndef __TRAITEMENT_H__
#define __TRAITEMENT_H__


void writePGM_25x14(const char *filename, int **data);
void writePGM_320x240(const char *filename, int **data);
int** centrage( int** depth , int X , int Y);
int ** MedianDepth( int** depth );
float Moyenne ( int** depth, int _x, int _y , int dx, int dy);
void RempliMatriceReelle(int **Mat,FILE *pf);
int** seuillage(int** depth , int X , int Y);

#endif
