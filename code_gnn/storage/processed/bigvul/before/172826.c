bool btif_config_exist(const char *section, const char *key) {
  assert(config != NULL);
  assert(section != NULL);
  assert(key != NULL);

  pthread_mutex_lock(&lock);
 bool ret = config_has_key(config, section, key);
  pthread_mutex_unlock(&lock);

 return ret;
}
