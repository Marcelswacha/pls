#ifndef NODE_H
#define NODE_H

#include <stdint.h>

enum file_type
{
    DIRECTORY,
    REGULAR_FILE,
    SYMLINK,
    SOMETHING_ELSE
};

struct node
{
    char* path;
    uint64_t size;
    enum file_type type;
    struct node** children;
    unsigned depth;
};

struct node* node_create(const char* path, unsigned depth);
void node_free(struct node* n);
int node_is_proper_dir(struct node* n);
void build_tree(struct node* head);
void traverse_tree(struct node* head);
void free_tree(struct node* head);

#endif /* CONCAT_H_ */
