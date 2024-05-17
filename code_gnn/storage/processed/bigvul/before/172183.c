reactor_t *reactor_new(void) {
 reactor_t *ret = (reactor_t *)osi_calloc(sizeof(reactor_t));
 if (!ret)
 return NULL;

  ret->epoll_fd = INVALID_FD;
  ret->event_fd = INVALID_FD;

  ret->epoll_fd = epoll_create(MAX_EVENTS);
 if (ret->epoll_fd == INVALID_FD) {
    LOG_ERROR("%s unable to create epoll instance: %s", __func__, strerror(errno));
 goto error;
 }

  ret->event_fd = eventfd(0, 0);
 if (ret->event_fd == INVALID_FD) {
    LOG_ERROR("%s unable to create eventfd: %s", __func__, strerror(errno));
 goto error;
 }

  pthread_mutex_init(&ret->list_lock, NULL);
  ret->invalidation_list = list_new(NULL);
 if (!ret->invalidation_list) {
    LOG_ERROR("%s unable to allocate object invalidation list.", __func__);
 goto error;
 }

 struct epoll_event event;
  memset(&event, 0, sizeof(event));
  event.events = EPOLLIN;
  event.data.ptr = NULL;
 if (epoll_ctl(ret->epoll_fd, EPOLL_CTL_ADD, ret->event_fd, &event) == -1) {
    LOG_ERROR("%s unable to register eventfd with epoll set: %s", __func__, strerror(errno));
 goto error;
 }

 return ret;

error:;
  reactor_free(ret);
 return NULL;
}
