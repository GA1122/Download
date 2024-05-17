void reactor_stop(reactor_t *reactor) {
  assert(reactor != NULL);

  eventfd_write(reactor->event_fd, EVENT_REACTOR_STOP);
}
