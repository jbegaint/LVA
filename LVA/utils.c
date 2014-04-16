#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "matrix.h"

/**
 * @brief exit program with failure
 *
 * @param err errors to print, followed by format
 */
void die(char *err, ...)
{
	va_list e;

	va_start(e, err);
	vfprintf(stderr, err, e);
	va_end(e);

	exit(EXIT_FAILURE);
}


/**
 * @brief display the 32 bits for an int
 *
 * @param val the int
 */
void print_bits(int val)
{
	for (int i = 31; i >= 0; --i)
		fprintf(stdout, "%d", (val >> i) & 1);

	fprintf(stdout, "\n");
}
