void socket_unregister(socket_t *socket) {
  assert(socket != NULL);

 if (socket->reactor_object)
    reactor_unregister(socket->reactor_object);
  socket->reactor_object = NULL;
}
