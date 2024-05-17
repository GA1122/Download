bool config_remove_section(config_t *config, const char *section) {
  assert(config != NULL);
  assert(section != NULL);

 section_t *sec = section_find(config, section);
 if (!sec)
 return false;

 return list_remove(config->sections, sec);
}
