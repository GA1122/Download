const config_section_node_t *config_section_next(const config_section_node_t *node) {
  assert(node != NULL);
 return (const config_section_node_t *)list_next((const list_node_t *)node);
}
