bool btif_config_get_int(const char *section, const char *key, int *value) {
  assert(config != NULL);
  assert(section != NULL);
  assert(key != NULL);
  assert(value != NULL);

  pthread_mutex_lock(&lock);
 bool ret = config_has_key(config, section, key);
 if (ret)
 *value = config_get_int(config, section, key, *value);
  pthread_mutex_unlock(&lock);

 return ret;
}
