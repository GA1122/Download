bool config_remove_key(config_t *config, const char *section, const char *key) {
  assert(config != NULL);
  assert(section != NULL);
  assert(key != NULL);

 section_t *sec = section_find(config, section);
 entry_t *entry = entry_find(config, section, key);
 if (!sec || !entry)
 return false;

 return list_remove(sec->entries, entry);
}
