void BluetoothSocketListenFunction::OnCreateService(
    scoped_refptr<device::BluetoothSocket> socket) {
  DCHECK_CURRENTLY_ON(work_thread_id());

  BluetoothApiSocket* api_socket = GetSocket(socket_id());
  if (!api_socket) {
    Respond(Error(kSocketNotFoundError));
    return;
  }

  api_socket->AdoptListeningSocket(socket,
                                   device::BluetoothUUID(uuid()));
  socket_event_dispatcher_->OnSocketListen(extension_id(), socket_id());
  Respond(ArgumentList(CreateResults()));
}
