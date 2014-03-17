#ifndef _PGM_FILE_H_
#define _PGM_FILE_H_

#include "matrix.h"

matrix_t *read_pgm_file(char *fileName);
/*int write_pgm_file(char *fileName, IMAGE_T * p_img, char *comment);*/

#endif
