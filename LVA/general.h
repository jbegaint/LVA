#ifndef _GENERAL_H_
#define _GENERAL_H_

#include "matrix.h"

#define CFG_FILE "lva.cfg"

#define FILE_ONI_TEST "./share/ONI/jb.oni"
#define FILE_PGM_TEST "./share/lva.pgm"

#define PIXELS_X 160
#define PIXELS_Y 120

#define ONI_PIXELS_X 320
#define ONI_PIXELS_Y 240

#define N_ROWS 14
#define N_COLS 25
#define N_GPIOS 4

#define N_LEVELS 4

/* thread related stuff */
#define THREAD_RUNNING 1
#define THREAD_PAUSED 0
#define THREAD_QUIT -1

typedef struct thread_info_t {
	matrix_t *matrix;
	int *thread_status;
	const char *filepath;
} thread_info_t;

#endif