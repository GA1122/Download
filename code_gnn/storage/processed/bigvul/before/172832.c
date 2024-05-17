bool btif_config_has_section(const char *section) {
  assert(config != NULL);
  assert(section != NULL);

  pthread_mutex_lock(&lock);
 bool ret = config_has_section(config, section);
  pthread_mutex_unlock(&lock);

 return ret;
}
