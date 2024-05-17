const char *config_section_name(const config_section_node_t *node) {
  assert(node != NULL);
 const list_node_t *lnode = (const list_node_t *)node;
 const section_t *section = (const section_t *)list_node(lnode);
 return section->name;
}
