socket_t *socket_new(void) {
 socket_t *ret = (socket_t *)osi_calloc(sizeof(socket_t));
 if (!ret) {
    LOG_ERROR("%s unable to allocate memory for socket.", __func__);
 goto error;
 }

  ret->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 if (ret->fd == INVALID_FD) {
    LOG_ERROR("%s unable to create socket: %s", __func__, strerror(errno));
 goto error;
 }

 int enable = 1;
 if (setsockopt(ret->fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1) {
    LOG_ERROR("%s unable to set SO_REUSEADDR: %s", __func__, strerror(errno));
 goto error;
 }

 return ret;

error:;
 if (ret)
    close(ret->fd);
  osi_free(ret);
 return NULL;
}
