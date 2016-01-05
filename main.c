#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "concat.h"

int listdir(const char* path, void (*decorator)(const char* path))
{
    struct dirent **namelist;
    int i, n;
    char resolved_path[1024];

    realpath(path, resolved_path);
    n = scandir(resolved_path, &namelist, 0, alphasort);
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

void decorate(const char* path)
{
    struct stat buf;

    stat(path, &buf);
    if (S_ISREG(buf.st_mode))
        printf("%-80s%lld\n", path, (long long)buf.st_size);
    else
        printf("%s\n", path);
}

int main(int argc, char** argv)
{
	int counter = 1;
	if (argc == 1)
		return listdir(".", decorate);

	while (++counter <= argc) {
		printf("\nListing %s:\n", argv[counter-1]);
		listdir(argv[counter-1], decorate);
	}
	
	return 0;
}
