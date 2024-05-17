static struct node *lookup_child_by_name_locked(struct node *node, const char *name)
{
 for (node = node->child; node; node = node->next) {
  
 if (!strcmp(name, node->name) && !node->deleted) {
 return node;
 }
 }
 return 0;
}
