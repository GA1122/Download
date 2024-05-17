bool config_get_bool(const config_t *config, const char *section, const char *key, bool def_value) {
  assert(config != NULL);
  assert(section != NULL);
  assert(key != NULL);

 entry_t *entry = entry_find(config, section, key);
 if (!entry)
 return def_value;

 if (!strcmp(entry->value, "true"))
 return true;
 if (!strcmp(entry->value, "false"))
 return false;

 return def_value;
}
