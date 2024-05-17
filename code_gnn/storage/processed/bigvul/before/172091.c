void GKI_enable(void) {
  pthread_mutex_unlock(&gki_cb.lock);
}
