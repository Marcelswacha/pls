#include <stdlib.h>

#include "concat.h"

char* concat(const char *first, const char* second)
{
    size_t len1 = strlen(first);
    size_t len2 = strlen(second);

    char * s = malloc(len1 + len2 + 2);
    memcpy(s, first, len1);
    s[len1] = '/';
    memcpy(s + len1 + 1, second, len2 + 1); // includes terminating null
    return s;
}

int is_dot(const char* path)
{
    int result = 0;
    char* copy;
    char* name;
    copy = strdup(path);

    name = basename(copy);
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
        result = 1;

    free(copy);
    return result;
}

