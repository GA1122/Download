void reactor_free(reactor_t *reactor) {
 if (!reactor)
 return;

  list_free(reactor->invalidation_list);
  close(reactor->event_fd);
  close(reactor->epoll_fd);
  osi_free(reactor);
}
