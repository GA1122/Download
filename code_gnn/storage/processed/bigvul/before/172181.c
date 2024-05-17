bool reactor_change_registration(reactor_object_t *object,
 void (*read_ready)(void *context),
 void (*write_ready)(void *context)) {
  assert(object != NULL);

 struct epoll_event event;
  memset(&event, 0, sizeof(event));
 if (read_ready)
    event.events |= (EPOLLIN | EPOLLRDHUP);
 if (write_ready)
    event.events |= EPOLLOUT;
  event.data.ptr = object;

 if (epoll_ctl(object->reactor->epoll_fd, EPOLL_CTL_MOD, object->fd, &event) == -1) {
    LOG_ERROR("%s unable to modify interest set for fd %d: %s", __func__, object->fd, strerror(errno));
 return false;
 }

  pthread_mutex_lock(&object->lock);
  object->read_ready = read_ready;
  object->write_ready = write_ready;
  pthread_mutex_unlock(&object->lock);

 return true;
}
