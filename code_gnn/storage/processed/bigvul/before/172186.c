reactor_status_t reactor_start(reactor_t *reactor) {
  assert(reactor != NULL);
 return run_reactor(reactor, 0);
}
