SocketInfo CreateSocketInfo(int socket_id, BluetoothApiSocket* socket) {
  DCHECK_CURRENTLY_ON(BluetoothApiSocket::kThreadId);
  SocketInfo socket_info;
  socket_info.socket_id = socket_id;
  if (socket->name()) {
    socket_info.name.reset(new std::string(*socket->name()));
  }
  socket_info.persistent = socket->persistent();
  if (socket->buffer_size() > 0) {
    socket_info.buffer_size.reset(new int(socket->buffer_size()));
  }
  socket_info.paused = socket->paused();
  socket_info.connected = socket->IsConnected();

  if (socket->IsConnected())
    socket_info.address.reset(new std::string(socket->device_address()));
  socket_info.uuid.reset(new std::string(socket->uuid().canonical_value()));

  return socket_info;
}
