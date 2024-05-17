const config_section_node_t *config_section_begin(const config_t *config) {
  assert(config != NULL);
 return (const config_section_node_t *)list_begin(config->sections);
}
