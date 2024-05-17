struct node *create_node_locked(struct fuse* fuse,
 struct node *parent, const char *name, const char* actual_name)
{
 struct node *node;
 size_t namelen = strlen(name);

 if (fuse->global->inode_ctr == 0) {
        ERROR("No more inode numbers available");
 return NULL;
 }

    node = calloc(1, sizeof(struct node));
 if (!node) {
 return NULL;
 }
    node->name = malloc(namelen + 1);
 if (!node->name) {
        free(node);
 return NULL;
 }
    memcpy(node->name, name, namelen + 1);
 if (strcmp(name, actual_name)) {
        node->actual_name = malloc(namelen + 1);
 if (!node->actual_name) {
            free(node->name);
            free(node);
 return NULL;
 }
        memcpy(node->actual_name, actual_name, namelen + 1);
 }
    node->namelen = namelen;
    node->nid = ptr_to_id(node);
    node->ino = fuse->global->inode_ctr++;
    node->gen = fuse->global->next_generation++;

    node->deleted = false;

    derive_permissions_locked(fuse, parent, node);
    acquire_node_locked(node);
    add_node_to_parent_locked(node, parent);
 return node;
}
