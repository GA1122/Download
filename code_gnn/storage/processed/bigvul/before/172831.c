bool btif_config_get_str(const char *section, const char *key, char *value, int *size_bytes) {
  assert(config != NULL);
  assert(section != NULL);
  assert(key != NULL);
  assert(value != NULL);
  assert(size_bytes != NULL);

  pthread_mutex_lock(&lock);
 const char *stored_value = config_get_string(config, section, key, NULL);
  pthread_mutex_unlock(&lock);

 if (!stored_value)
 return false;

  strlcpy(value, stored_value, *size_bytes);
 *size_bytes = strlen(value) + 1;

 return true;
}
