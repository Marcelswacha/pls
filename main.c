#include <stdio.h>
#include <getopt.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "concat.h"

#define NO_DOT 1
static int no_dot = 0;

void decorate(const char* path)
{
    struct stat buf;

    stat(path, &buf);
    if (S_ISREG(buf.st_mode))
        printf("%-80s%lld\n", path, (long long)buf.st_size);
    else
        printf("%s\n", path);
}

int no_dot_filter(const struct dirent* entry)
{
    return (entry->d_name[0] == '.') ? 0 : 1;
}

int filter_collection(const struct dirent* entry)
{
    int result = 1;
    if (no_dot) {
        result = no_dot_filter(entry);
    }

    return result;
}

int listdir(const char* path, void (*decorator)(const char* path))
{
    struct dirent **namelist;
    int i, n;
    char resolved_path[1024];

    realpath(path, resolved_path);
    printf("%s:\n", resolved_path);

    n = scandir(resolved_path, &namelist, filter_collection, alphasort);
    if (n < 0) {
        perror("scandir");
        return -1;
    }
    else {
        for (i = 0; i < n; ++i) {
            /* get absolute path */
            char* absolute_path = concat(resolved_path, "/",
                                         namelist[i]->d_name, NULL);

            /* print info */
            (*decorator)(absolute_path);

            /* clean up */
            free(namelist[i]);
            free(absolute_path);
        }
    }

    free(namelist);
    return 0;
}


static struct option long_options[] = {
    {"no-dot", 0, NULL, NO_DOT},
    {NULL, 0, NULL, 0}
};

int main(int argc, char** argv)
{
    int next_option;
    int i;

    if (argc == 1)
        listdir(".", decorate);

	do {
        next_option = getopt_long(argc, argv,
                                  "", long_options, NULL);
        switch (next_option)
        {
	        case NO_DOT:
	            no_dot = 1;
	            break;
	        case -1:
	            break;
	        default:
	            abort();
		}
	} while (next_option != -1);

	for (i = optind; i < argc; ++i)
		listdir(argv[i], decorate);

	return 0;
}
