void socket_register(socket_t *socket, reactor_t *reactor, void *context, socket_cb read_cb, socket_cb write_cb) {
  assert(socket != NULL);

  socket_unregister(socket);

  socket->read_ready = read_cb;
  socket->write_ready = write_cb;
  socket->context = context;

 void (*read_fn)(void *) = (read_cb != NULL) ? internal_read_ready : NULL;
 void (*write_fn)(void *) = (write_cb != NULL) ? internal_write_ready : NULL;

  socket->reactor_object = reactor_register(reactor, socket->fd, socket, read_fn, write_fn);
}
