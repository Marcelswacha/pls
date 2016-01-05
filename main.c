#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

int listdir(const char* path)
{
    struct dirent **namelist;
    int i, n;

    n = scandir(path, &namelist, 0, alphasort);
    if (n < 0)
        perror("scandir");
    else {
        for (i = 0; i < n; ++i) {
            printf("%s\n", namelist[i]->d_name);
            free(namelist[i]);
        }
    }
    free(namelist);
}

int main(int argc, char** argv)
{
	int counter = 1;
	if (argc == 1)
		listdir(".");

	while (++counter <= argc) {
		printf("\nListing %s:\n", argv[counter-1]);
		listdir(argv[counter-1]);
	}
	
	return 0;
}
