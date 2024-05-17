bool btif_config_set_int(const char *section, const char *key, int value) {
  assert(config != NULL);
  assert(section != NULL);
  assert(key != NULL);

  pthread_mutex_lock(&lock);
  config_set_int(config, section, key, value);
  pthread_mutex_unlock(&lock);

 return true;
}
