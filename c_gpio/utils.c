#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void die(char *err, ...)
{
	va_list e;

	va_start(e, err);
	vfprintf(stderr, err, e);
	va_end(e);

	exit(EXIT_FAILURE);
}