static void acquire_node_locked(struct node* node)
{
    node->refcount++;
    TRACE("ACQUIRE %p (%s) rc=%d\n", node, node->name, node->refcount);
}
