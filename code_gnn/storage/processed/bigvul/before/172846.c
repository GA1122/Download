static future_t *clean_up(void) {
  btif_config_flush();

  alarm_free(alarm_timer);
  config_free(config);
  pthread_mutex_destroy(&lock);
  alarm_timer = NULL;
  config = NULL;
 return future_new_immediate(FUTURE_SUCCESS);
}
