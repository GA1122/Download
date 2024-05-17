static void *reactor_thread(void *ptr) {
 reactor_t *reactor = (reactor_t *)ptr;

  thread_running = true;
  reactor_start(reactor);
  thread_running = false;

 return NULL;
}
