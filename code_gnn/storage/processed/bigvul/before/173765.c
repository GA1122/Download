static void release_node_locked(struct node* node)
{
    TRACE("RELEASE %p (%s) rc=%d\n", node, node->name, node->refcount);
 if (node->refcount > 0) {
        node->refcount--;
 if (!node->refcount) {
            TRACE("DESTROY %p (%s)\n", node, node->name);
            remove_node_from_parent_locked(node);

  
            memset(node->name, 0xef, node->namelen);
            free(node->name);
            free(node->actual_name);
            memset(node, 0xfc, sizeof(*node));
            free(node);
 }
 } else {
        ERROR("Zero refcnt %p\n", node);
 }
}
