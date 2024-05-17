reactor_status_t reactor_run_once(reactor_t *reactor) {
  assert(reactor != NULL);
 return run_reactor(reactor, 1);
}
