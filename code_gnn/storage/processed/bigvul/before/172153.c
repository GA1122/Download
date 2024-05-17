bool config_has_key(const config_t *config, const char *section, const char *key) {
  assert(config != NULL);
  assert(section != NULL);
  assert(key != NULL);

 return (entry_find(config, section, key) != NULL);
}
