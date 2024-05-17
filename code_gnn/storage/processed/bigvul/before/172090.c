void GKI_disable(void) {
  pthread_mutex_lock(&gki_cb.lock);
}
