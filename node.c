#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "node.h"
#include "concat.h"
#include "colors.h"
#include "options.h"
#include "stack.h"

struct node* node_create(const char* path, unsigned depth)
{
    struct node* n = malloc(sizeof(struct node));

    if (n) {
        n->path = strdup(path);
        n->children = NULL;
        n->depth = depth;
    }

    return n;
}

char* format_bytes (uint64_t  bytes)
{
    char* result = malloc(128 * sizeof(char));
    const char* suffix[5] = { "B", "KB", "MB", "GB", "TB" };

    double dblSByte = bytes;
    int i;
    for (i = 0; i < 5 && bytes >= 1024; i++, bytes /= 1024)
    {
        dblSByte = bytes / 1024.0;
    }

    sprintf(result, "%10.2lf %s", dblSByte, suffix[i]);
    return result;
}

void node_print(struct node* n, char** buffer)
{
    char* copy = strdup(n->path);
    char* size = format_bytes(n->size);
    int l;

    switch (n->type) {
    case DIRECTORY:
        l = sprintf(*buffer, ANSI_COLOR_CYAN "\t%-60s%s" ANSI_COLOR_RESET"\n",
        basename(copy), size);
        *buffer += l;
        break;
    default:
        l = sprintf(*buffer, "\t%-60s%s\n", basename(copy), size);
        *buffer += l;
        break;
    }
    free(copy);
    free(size);
}

void head_print(struct node* head, char** buffer)
{
    char* size = format_bytes(head->size);
    int n = sprintf(*buffer, ANSI_COLOR_BLUE "%-67s %s" ANSI_COLOR_RESET"\n", head->path, size);
    *buffer += n;
    free(size);
}

int node_is_proper_dir(struct node* n)
{
    return (n->type == DIRECTORY && is_dot(n->path) == 0) ? 1 : 0;
}

static void get_node_info(struct node* n)
{
    struct stat buf;

        /* get info of the node */
        lstat(n->path, &buf);

        n->size= buf.st_size;
        if (S_ISDIR(buf.st_mode))
            n->type = DIRECTORY;
        else if (S_ISREG(buf.st_mode))
            n->type = REGULAR_FILE;
        else if (S_ISLNK(buf.st_mode))
            n->type = SYMLINK;
        else
            n->type = SOMETHING_ELSE;
}

static void do_preprocessing(struct node* n);
static void do_postprocessing(struct node* n);

void build_tree(struct node* head)
{
    struct stack* stack_pre = stack_create_default();
    struct stack* stack_post = stack_create_default();

    stack_push(stack_pre, head);

    struct node* current;
    while(stack_not_empty(stack_pre) == 1) {
        current = stack_top(stack_pre);
        stack_pop(stack_pre);

        /* do preprocessing */
        do_preprocessing(current);

        /* push it on post */
        stack_push(stack_post, current);

        /* push current's children to pre */
        if (current->children)
            for (int i = 0; current->children[i] != NULL; ++i)
                stack_push(stack_pre, current->children[i]);
    }

    /* Release pre stack */
    stack_destroy(stack_pre);

    while(stack_not_empty(stack_post) == 1) {
        struct node* current = stack_top(stack_post);
        stack_pop(stack_post);

        /* do post processing */
        do_postprocessing(current);
    }

    /* Release post_stack */
    stack_destroy(stack_post);
}

static void do_preprocessing(struct node* head)
{
    /* inspect element */
    get_node_info(head);

    if (node_is_proper_dir(head)) {
        struct dirent** namelist;

        /* scan directory */
        int n = scandir(head->path, &namelist, 0, NULL);
        if (n < 0)
            printf("%s: %s\n", head->path, strerror(errno));
        else {
            /*build children nodes*/
            head->children = malloc((n + 1) * sizeof(struct node*));
            head->children[n] = NULL;
            if (head->children) {
                int i;
                for (i = 0; i < n; ++i) {
                    /* fill children names */
                    char * child_path = concat(head->path, "/",
                            namelist[i]->d_name,
                            NULL);
                    head->children[i] = node_create(child_path,
                            head->depth + 1);

                    /* clean up */
                    free(child_path);
                    free(namelist[i]);
                }
            }
            free(namelist);
        }
    }
}

static void do_postprocessing(struct node* head)
{
    if (head->children)
        for (int i = 0; head->children[i] != NULL; ++i)
            head->size += head->children[i]->size;
}

int mycmp(const void *s1, const void *s2)
{
    const struct node* l = *(const struct node **)s1;
    const struct node *r = *(const struct node **)s2;

    if (l->type == DIRECTORY && r->type != DIRECTORY) return -1;
    else if (l->type != DIRECTORY && r->type == DIRECTORY) return 1;
    else if (l->size > r->size) return -1;
    else if (l->size < r->size) return 1;

    return strcmp(l->path, r->path);
}

void traverse_tree(struct node* head, int depth, char** buffer)
{
    /* print head */
    head_print(head, buffer);

    /* sort children */
    int size = 0;
    for (int i = 0; head->children[i] != NULL; ++i)
        size += 1;

    qsort(head->children, size, sizeof(struct node*), mycmp);

    /* print childrens */
    for (int i = 0; head->children[i] != NULL; ++i)
        if (is_dot(head->children[i]->path) == 0 || opt_show_dot)
            node_print(head->children[i], buffer);
    int n = sprintf(*buffer, "\n");
    *buffer += n;

    /* traverse dirs only */

    if (opt_recursive && depth < opt_max_depth)
        for (int i = 0; head->children[i] != NULL; ++i)
            if (node_is_proper_dir(head->children[i]))
                traverse_tree(head->children[i], depth+1, buffer);
}
