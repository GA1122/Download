void SetSocketProperties(BluetoothApiSocket* socket,
                         SocketProperties* properties) {
  if (properties->name.get()) {
    socket->set_name(*properties->name);
  }
  if (properties->persistent.get()) {
    socket->set_persistent(*properties->persistent);
  }
  if (properties->buffer_size.get()) {
    socket->set_buffer_size(*properties->buffer_size);
  }
}
