#include <stdio.h>
#include <getopt.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "concat.h"

static int no_dot = 1;

void decorate(const char* parent, const char* child)
{
    /* get absolute path */
    char* absolute_path = concat(parent, "/", child, NULL);

    struct stat buf;

    stat(absolute_path, &buf);
    if (S_ISREG(buf.st_mode))
        printf("\t%-80s%lld\n", child, (long long)buf.st_size);
    else
        printf("\t%-80s\n", child);
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

int listdir(const char* path)
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
            /* print info */
            decorate(resolved_path, namelist[i]->d_name);

            /* clean up */
            free(namelist[i]);
        }
    }

    free(namelist);
    return 0;
}

static struct option long_options[] = {
    {"dot", 0, NULL, 'd'},
    {NULL, 0, NULL, 0}
};

int main(int argc, char** argv)
{
    int next_option;
    int i;

    do {
        next_option = getopt_long(argc, argv, "d", long_options, NULL);
        switch (next_option) {
        case 'd':
            no_dot = 0;
            break;
        case -1:
            break;
        default:
            abort();
        }
    } while (next_option != -1);

    if (optind == argc)
        listdir(".");
    else
        for (i = optind; i < argc; ++i)
            listdir(argv[i]);

    return 0;
}
