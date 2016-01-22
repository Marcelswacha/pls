#include <stdlib.h>

#include "concat.h"

#define CONCAT_ARRAY_SIZE 1024*1024*128

static char CHAR_ARRAY[CONCAT_ARRAY_SIZE];

static char* get_path_memory(const size_t size)
{
    static int array_ptr = 0;
    if (array_ptr + 1 + size >= CONCAT_ARRAY_SIZE )
        return malloc(size * sizeof(char));

    int ptr_before = array_ptr;
    array_ptr += size;

    return CHAR_ARRAY + ptr_before;
}

char* concat(const char *first, const char* second)
{
    size_t len1 = strlen(first);
    size_t len2 = strlen(second);
    size_t total_size = len1 + len2 + 2;

    char * s = get_path_memory(total_size);
    memcpy(s, first, len1);
    s[len1] = '/';
    memcpy(s + len1 + 1, second, len2 + 1); // includes terminating null
    return s;
}

int is_dot(const char* name)
{
    int result = 0;
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
        result = 1;

    return result;
}

int is_dot_full(const char* path)
{
    int result = 0;
    char* copy = strdup(path);
    result = is_dot(basename(copy));

    free(copy);
    return result;
}

