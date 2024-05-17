void config_set_bool(config_t *config, const char *section, const char *key, bool value) {
  assert(config != NULL);
  assert(section != NULL);
  assert(key != NULL);

  config_set_string(config, section, key, value ? "true" : "false");
}
