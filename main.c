#include <stdlib.h>
#include <getopt.h>

#include "node.h"

int listdir(const char* path)
{
    char resolved_path[1024];
    realpath(path, resolved_path);

    struct node* head = node_create(resolved_path, 0);
    build_tree(head);
    if (head->type == DIRECTORY) {
        traverse_tree(head);
    } else
        node_print(head);

    return 0;
}

static int no_dot = 0;
static struct option long_options[] = {
    {"no-dot", 0, NULL, 'd'},
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
            no_dot = 1;
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
