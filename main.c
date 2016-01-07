#include <stdio.h>
#include <dirent.h>
#include <getopt.h>
#include <libgen.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>

#include "colors.h"
#include "concat.h"

static int no_dot = 1;

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

uint64_t get_size(const char* path)
{
    struct stat buf;
    struct dirent **namelist;
    uint64_t size = 0u;
    char* absolute_path;
    int i, n;

    lstat(path, &buf);
    size += (unsigned long long )buf.st_size;
    if ((S_ISDIR(buf.st_mode)) && (is_dot(path) == 0)) {
        /* enter directory and get size of each node */
        n = scandir(path, &namelist, 0, 0);
        if (n < 0) {
            perror("scandir");
            printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET"\n", path);
        } else {
            for (i = 0; i < n; ++i) {
                /* get absolute path */
                absolute_path = concat(path, "/", namelist[i]->d_name, NULL);

                /* count the size */
                size += get_size(absolute_path);

                /* clean up */
                free(namelist[i]);
                free(absolute_path);
            }
            free(namelist);
        }
    }

    return size;
}

void decorate(const char* parent, const char* child)
{
    /* get absolute path */
    char* absolute_path = concat(parent, "/", child, NULL);

    struct stat buf;

    lstat(absolute_path, &buf);
    uint64_t size = get_size(absolute_path);

    if (S_ISDIR(buf.st_mode))
        printf(ANSI_COLOR_CYAN "\t%-40s%llu" ANSI_COLOR_RESET"\n",
                        child, size);
    else
        printf("\t%-40s%llu\n", child, size);

    free(absolute_path);
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
