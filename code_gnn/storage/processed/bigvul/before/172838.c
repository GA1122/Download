const char *btif_config_section_name(const btif_config_section_iter_t *section) {
  assert(config != NULL);
  assert(section != NULL);
 return config_section_name((const config_section_node_t *)section);
}
