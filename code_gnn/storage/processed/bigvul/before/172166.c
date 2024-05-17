void config_set_int(config_t *config, const char *section, const char *key, int value) {
  assert(config != NULL);
  assert(section != NULL);
  assert(key != NULL);

 char value_str[32] = { 0 };
  sprintf(value_str, "%d", value);
  config_set_string(config, section, key, value_str);
}
