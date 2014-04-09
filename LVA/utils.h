#ifndef _UTILS_H_
#define _UTILS_H_

#define UNUSED(x) (void)(x)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define THREAD_EXIT -1

typedef struct thread_info_t {
	matrix_t *matrix;
	const char *filepath;
	int *next_frame;
} thread_info_t;

void die(char *err, ...);
void print_bits(int val);

#endif
