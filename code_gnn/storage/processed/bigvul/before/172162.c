const config_section_node_t *config_section_end(const config_t *config) {
  assert(config != NULL);
 return (const config_section_node_t *)list_end(config->sections);
}
