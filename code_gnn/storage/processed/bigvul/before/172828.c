bool btif_config_get_bin(const char *section, const char *key, uint8_t *value, size_t *length) {
  assert(config != NULL);
  assert(section != NULL);
  assert(key != NULL);
  assert(value != NULL);
  assert(length != NULL);

  pthread_mutex_lock(&lock);
 const char *value_str = config_get_string(config, section, key, NULL);
  pthread_mutex_unlock(&lock);

 if (!value_str)
 return false;

 size_t value_len = strlen(value_str);
 if ((value_len % 2) != 0 || *length < (value_len / 2))
 return false;

 for (size_t i = 0; i < value_len; ++i)
 if (!isxdigit(value_str[i]))
 return false;

 for (*length = 0; *value_str; value_str += 2, *length += 1)
    sscanf(value_str, "%02hhx", &value[*length]);

 return true;
}
