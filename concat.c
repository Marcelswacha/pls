#include <stdlib.h>

#include "concat.h"

char* concat (const char *str, ...)
{
  va_list ap, ap2;
  size_t total = 1;
  const char *s;
  char *result;

  va_start (ap, str);
  va_copy (ap2, ap);

  /* Determine how much space we need.  */
  for (s = str; s != NULL; s = va_arg (ap, const char *))
    total += strlen (s);

  va_end (ap);

  result = (char *) malloc (total);
  if (result != NULL)
    {
      result[0] = '\0';

      /* Copy the strings.  */
      for (s = str; s != NULL; s = va_arg (ap2, const char *))
        strcat (result, s);
    }

  va_end (ap2);

  return result;
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

    free (copy);
    return result;
}

