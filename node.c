#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "node.h"
#include "concat.h"
#include "colors.h"
#include "options.h"

struct node* node_create(const char* path, unsigned depth)
{
    struct node* n = NULL;

    n = malloc(sizeof(struct node));

    if (n) {
        n->path = strdup(path);
        n->children = NULL;
        n->depth = depth;
    }

    return n;
}

void node_free(struct node* n)
{
    free(n->path);
    if (n->children)
        free(n->children);
    free(n);
}

void node_print(struct node* n)
{
    char* copy;
    copy = strdup(n->path);
    switch (n->type) {
    case DIRECTORY:
        printf(ANSI_COLOR_CYAN "\t%-40s%llu" ANSI_COLOR_RESET"\n",
        basename(copy), n->size);
        break;
    default:
        printf("\t%-40s%llu\n", basename(copy), n->size);
        break;
    }
    free(copy);
}

int node_is_proper_dir(struct node* n)
{
    return (n->type == DIRECTORY && is_dot(n->path) == 0) ? 1 : 0;
}

void build_tree(struct node* head)
{
    struct stat buf;
    struct dirent** namelist;
    int n, i;

    /* get info of the node */
    lstat(head->path, &buf);

    uint64_t size = buf.st_size;
    if (S_ISDIR(buf.st_mode))
        head->type = DIRECTORY;
    else if (S_ISREG(buf.st_mode))
        head->type = REGULAR_FILE;
    else if (S_ISLNK(buf.st_mode))
        head->type = SYMLINK;
    else
        head->type = SOMETHING_ELSE;

    /* if it is the proper directory, build trees on its children */
    if (node_is_proper_dir(head)) {
        /* scan directory */
        n = scandir(head->path, &namelist, 0, alphasort);
        if (n < 0)
            perror("scandir");
        else {
            /*build children nodes*/
            head->children = malloc((n + 1) * sizeof(struct node*));
            head->children[n] = NULL;
            if (head->children) {
                for (i = 0; i < n; ++i) {
                    /* fill children names */
                    char * child_path = concat(head->path, "/",
                            namelist[i]->d_name,
                            NULL);
                    head->children[i] = node_create(child_path,
                            head->depth + 1);

                    /* span tree recursively  - DFS*/
                    build_tree(head->children[i]);
                    size += head->children[i]->size;

                    /* clean up */
                    free(child_path);
                    free(namelist[i]);
                }
            }
            free(namelist);
        }
    }

    head->size = size;
}

void traverse_tree(struct node* head)
{
    /* BFS traversal */
    int i;

    /* print head */
    printf(ANSI_COLOR_BLUE "%s: %llu" ANSI_COLOR_RESET"\n", head->path,
            head->size);

    /* print childrens */
    for (i = 0; head->children[i] != NULL; ++i)
        if (is_dot(head->children[i]->path) == 0 || opt_show_dot)
            node_print(head->children[i]);
    printf("\n");

    /* traverse dirs only */
    if (opt_recursive)
        for (i = 0; head->children[i] != NULL; ++i)
            if (node_is_proper_dir(head->children[i]))
                traverse_tree(head->children[i]);
}
