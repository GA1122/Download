static void btif_config_write(void) {
  assert(config != NULL);
  assert(alarm_timer != NULL);

  pthread_mutex_lock(&lock);
 config_t *config_paired = config_new_clone(config);
  btif_config_remove_unpaired(config_paired);
  config_save(config_paired, CONFIG_FILE_PATH);
  config_free(config_paired);
  pthread_mutex_unlock(&lock);
}
