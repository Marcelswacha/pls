#ifndef NODE_H
#define NODE_H

#include <stdint.h>

enum file_type
{
    REGULAR_DIRECTORY, REGULAR_FILE, SYMLINK, SOMETHING_ELSE, DOT_DIRECTORY
};

struct node
{
    char* path;
    uint64_t size;
    enum file_type type;
    struct node** children;
    unsigned depth;
};

struct node* node_create(char* path, unsigned depth);
struct node* node_get(char* path, unsigned depth);
void node_print(struct node* n, char** buffer);
int node_is_proper_dir(struct node* n);
void build_tree(struct node* head);
void traverse_tree(struct node* head, int depth, char** buffer
        );
void free_tree(struct node* head);

#endif /* CONCAT_H_ */
