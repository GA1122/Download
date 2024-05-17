void reactor_unregister(reactor_object_t *obj) {
  assert(obj != NULL);

 reactor_t *reactor = obj->reactor;

 if (epoll_ctl(reactor->epoll_fd, EPOLL_CTL_DEL, obj->fd, NULL) == -1)
    LOG_ERROR("%s unable to unregister fd %d from epoll set: %s", __func__, obj->fd, strerror(errno));

 if (reactor->is_running && pthread_equal(pthread_self(), reactor->run_thread)) {
    reactor->object_removed = true;
 return;
 }

  pthread_mutex_lock(&reactor->list_lock);
  list_append(reactor->invalidation_list, obj);
  pthread_mutex_unlock(&reactor->list_lock);

  pthread_mutex_lock(&obj->lock);
  pthread_mutex_unlock(&obj->lock);
  pthread_mutex_destroy(&obj->lock);
  osi_free(obj);
}
