void BluetoothSocketAbstractConnectFunction::OnConnect(
    scoped_refptr<device::BluetoothSocket> socket) {
  DCHECK_CURRENTLY_ON(work_thread_id());

  BluetoothApiSocket* api_socket = GetSocket(params_->socket_id);
  if (!api_socket) {
    Respond(Error(kSocketNotFoundError));
    return;
  }

  api_socket->AdoptConnectedSocket(socket,
                                   params_->address,
                                   device::BluetoothUUID(params_->uuid));
  socket_event_dispatcher_->OnSocketConnect(extension_id(),
                                            params_->socket_id);

  Respond(ArgumentList(bluetooth_socket::Connect::Results::Create()));
}
