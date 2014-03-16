#ifndef _UTILS_H_
#define _UTILS_H_

#define UNUSED(x) (void)(x)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

void die(char *err, ...);
void print_bits(int val);
void writePGM_5x7(const char *filename, int **data);

#endif
