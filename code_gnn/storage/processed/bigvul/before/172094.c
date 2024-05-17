static future_t *init(void) {
  memset(&gki_cb, 0, sizeof(gki_cb));

 pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
  pthread_mutex_init(&gki_cb.lock, &attr);

  gki_buffer_init();
 return NULL;
}
