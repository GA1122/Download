static void internal_write_ready(void *context) {
  assert(context != NULL);

 socket_t *socket = (void *)context;
  socket->write_ready(socket, socket->context);
}
