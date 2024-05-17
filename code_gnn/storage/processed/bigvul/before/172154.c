bool config_has_section(const config_t *config, const char *section) {
  assert(config != NULL);
  assert(section != NULL);

 return (section_find(config, section) != NULL);
}
