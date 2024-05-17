static void join_reactor_thread() {
  pthread_join(thread, NULL);
}
