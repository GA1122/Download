static void remove_node_from_parent_locked(struct node* node)
{
 if (node->parent) {
 if (node->parent->child == node) {
            node->parent->child = node->parent->child->next;
 } else {
 struct node *node2;
            node2 = node->parent->child;
 while (node2->next != node)
                node2 = node2->next;
            node2->next = node->next;
 }
        release_node_locked(node->parent);
        node->parent = NULL;
        node->next = NULL;
 }
}
