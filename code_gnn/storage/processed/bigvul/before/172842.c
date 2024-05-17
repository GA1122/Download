bool btif_config_set_str(const char *section, const char *key, const char *value) {
  assert(config != NULL);
  assert(section != NULL);
  assert(key != NULL);
  assert(value != NULL);

  pthread_mutex_lock(&lock);
  config_set_string(config, section, key, value);
  pthread_mutex_unlock(&lock);

 return true;
}
