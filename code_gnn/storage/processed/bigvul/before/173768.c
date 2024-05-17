static int rename_node_locked(struct node *node, const char *name,
 const char* actual_name)
{
 size_t namelen = strlen(name);
 int need_actual_name = strcmp(name, actual_name);

  
 if (namelen > node->namelen) {
 char* new_name = realloc(node->name, namelen + 1);
 if (!new_name) {
 return -ENOMEM;
 }
        node->name = new_name;
 if (need_actual_name && node->actual_name) {
 char* new_actual_name = realloc(node->actual_name, namelen + 1);
 if (!new_actual_name) {
 return -ENOMEM;
 }
            node->actual_name = new_actual_name;
 }
 }

  
 if (need_actual_name) {
 if (!node->actual_name) {
            node->actual_name = malloc(namelen + 1);
 if (!node->actual_name) {
 return -ENOMEM;
 }
 }
        memcpy(node->actual_name, actual_name, namelen + 1);
 } else {
        free(node->actual_name);
        node->actual_name = NULL;
 }
    memcpy(node->name, name, namelen + 1);
    node->namelen = namelen;
 return 0;
}
