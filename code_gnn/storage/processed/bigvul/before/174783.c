 int btif_config_clear(void){
  assert(config != NULL);
  assert(alarm_timer != NULL);

  alarm_cancel(alarm_timer);

  pthread_mutex_lock(&lock);
  config_free(config);

  config = config_new_empty();
 if (config == NULL) {
    pthread_mutex_unlock(&lock);
 return false;
 }

 int ret = config_save(config, CONFIG_FILE_PATH);
  pthread_mutex_unlock(&lock);

   return ret;
 }
