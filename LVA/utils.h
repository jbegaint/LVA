#ifndef _UTILS_H_
#define _UTILS_H_

#include "matrix.h"

/* thread related stuff */
#define THREAD_RUNNING 1
#define THREAD_PAUSED 0
#define THREAD_QUIT -1

#define FILE_ONI_TEST "./share/ONI/jb.oni"
#define FILE_PGM_TEST "./share/lena.pgm"

typedef struct thread_info_t {
	matrix_t *matrix;
	int *thread_status;
	const char *filepath;
} thread_info_t;


/* general */
#define UNUSED(x) (void)(x)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

void die(char *err, ...);
void print_bits(int val);

#endif
