const btif_config_section_iter_t *btif_config_section_end(void) {
  assert(config != NULL);
 return (const btif_config_section_iter_t *)config_section_end(config);
}
