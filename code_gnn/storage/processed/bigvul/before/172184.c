reactor_object_t *reactor_register(reactor_t *reactor,
 int fd, void *context,
 void (*read_ready)(void *context),
 void (*write_ready)(void *context)) {
  assert(reactor != NULL);
  assert(fd != INVALID_FD);

 reactor_object_t *object = (reactor_object_t *)osi_calloc(sizeof(reactor_object_t));
 if (!object) {
    LOG_ERROR("%s unable to allocate reactor object: %s", __func__, strerror(errno));
 return NULL;
 }

  object->reactor = reactor;
  object->fd = fd;
  object->context = context;
  object->read_ready = read_ready;
  object->write_ready = write_ready;
  pthread_mutex_init(&object->lock, NULL);

 struct epoll_event event;
  memset(&event, 0, sizeof(event));
 if (read_ready)
    event.events |= (EPOLLIN | EPOLLRDHUP);
 if (write_ready)
    event.events |= EPOLLOUT;
  event.data.ptr = object;

 if (epoll_ctl(reactor->epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
    LOG_ERROR("%s unable to register fd %d to epoll set: %s", __func__, fd, strerror(errno));
    pthread_mutex_destroy(&object->lock);
    osi_free(object);
 return NULL;
 }

 return object;
}
