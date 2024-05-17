bool btif_config_set_bin(const char *section, const char *key, const uint8_t *value, size_t length) {
 const char *lookup = "0123456789abcdef";

  assert(config != NULL);
  assert(section != NULL);
  assert(key != NULL);

 if (length > 0)
      assert(value != NULL);

 char *str = (char *)osi_calloc(length * 2 + 1);
 if (!str)
 return false;

 for (size_t i = 0; i < length; ++i) {
    str[(i * 2) + 0] = lookup[(value[i] >> 4) & 0x0F];
    str[(i * 2) + 1] = lookup[value[i] & 0x0F];
 }

  pthread_mutex_lock(&lock);
  config_set_string(config, section, key, str);
  pthread_mutex_unlock(&lock);

  osi_free(str);
 return true;
}
