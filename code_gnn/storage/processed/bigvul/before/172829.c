size_t btif_config_get_bin_length(const char *section, const char *key) {
  assert(config != NULL);
  assert(section != NULL);
  assert(key != NULL);

  pthread_mutex_lock(&lock);
 const char *value_str = config_get_string(config, section, key, NULL);
  pthread_mutex_unlock(&lock);

 if (!value_str)
 return 0;

 size_t value_len = strlen(value_str);
 return ((value_len % 2) != 0) ? 0 : (value_len / 2);
}
