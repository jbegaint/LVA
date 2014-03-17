#ifndef _PGM_FILE_H_
#define _PGM_FILE_H_

#include "matrix.h"

matrix_t *read_pgm_file(char *fileName);
int write_pgm_file(char *fileName, matrix_t *p_img, char *comment);

#endif
