static void internal_read_ready(void *context) {
  assert(context != NULL);

 socket_t *socket = (void *)context;
  socket->read_ready(socket, socket->context);
}
