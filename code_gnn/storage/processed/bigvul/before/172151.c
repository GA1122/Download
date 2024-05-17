int config_get_int(const config_t *config, const char *section, const char *key, int def_value) {
  assert(config != NULL);
  assert(section != NULL);
  assert(key != NULL);

 entry_t *entry = entry_find(config, section, key);
 if (!entry)
 return def_value;

 char *endptr;
 int ret = strtol(entry->value, &endptr, 0);
 return (*endptr == '\0') ? ret : def_value;
}
