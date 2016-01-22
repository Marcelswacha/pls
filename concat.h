#ifndef CONCAT_H_
#define CONCAT_H_

#include <stdio.h>
#include <libgen.h>
#include <stdarg.h>
#include <string.h>

char* concat(const char *first, const char* second);
int is_dot(const char* name);
int is_dot_full(const char* path);

#endif /* CONCAT_H_ */
