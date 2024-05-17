const btif_config_section_iter_t *btif_config_section_next(const btif_config_section_iter_t *section) {
  assert(config != NULL);
  assert(section != NULL);
 return (const btif_config_section_iter_t *)config_section_next((const config_section_node_t *)section);
}
