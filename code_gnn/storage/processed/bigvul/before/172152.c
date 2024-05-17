const char *config_get_string(const config_t *config, const char *section, const char *key, const char *def_value) {
  assert(config != NULL);
  assert(section != NULL);
  assert(key != NULL);

 entry_t *entry = entry_find(config, section, key);
 if (!entry)
 return def_value;

 return entry->value;
}
