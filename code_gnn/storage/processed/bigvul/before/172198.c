socket_t *socket_new_from_fd(int fd) {
  assert(fd != INVALID_FD);

 socket_t *ret = (socket_t *)osi_calloc(sizeof(socket_t));
 if (!ret) {
    LOG_ERROR("%s unable to allocate memory for socket.", __func__);
 return NULL;
 }

  ret->fd = fd;
 return ret;
}
