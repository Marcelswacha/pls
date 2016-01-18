#include <stdlib.h>
#include <getopt.h>
#include <limits.h>

#include "node.h"
#include "options.h"

int listdir(const char* path)
{
    char resolved_path[1024];
    realpath(path, resolved_path);

    struct node* head = node_create(resolved_path, 0);
    build_tree(head);
    if (head->type == DIRECTORY) {
        traverse_tree(head, 0);
    } else
        node_print(head);

    return 0;
}

static struct option long_options[] = { { "show-dot", 0, NULL, 's' },
        { "recursive", 0, NULL, 'r' }, { "depth", 0, NULL, 'd' },
        { "help", 0, NULL, 'h' }, { NULL, 0,
        NULL, 0 } };

int opt_show_dot = 0;
int opt_recursive = 0;
int opt_max_depth = INT_MAX;

void print_help()
{
}

int main(int argc, char** argv)
{
    int next_option;
    int i;

    do {
        next_option = getopt_long(argc, argv, "srd:h", long_options, NULL);
        switch (next_option) {
        case 's':
            opt_show_dot = 1;
            break;
        case 'r':
            opt_recursive = 1;
            break;
        case 'd':
            opt_max_depth = atoi(optarg);
            break;
        case 'h':
            print_help();
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
