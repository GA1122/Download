ExtensionFunction::ResponseAction BluetoothSocketDisconnectFunction::Run() {
  DCHECK_CURRENTLY_ON(work_thread_id());

  auto params = bluetooth_socket::Disconnect::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(params.get());

  BluetoothApiSocket* socket = GetSocket(params->socket_id);
  if (!socket)
    return RespondNow(Error(kSocketNotFoundError));

  socket->Disconnect(base::Bind(&BluetoothSocketDisconnectFunction::OnSuccess,
                                this));
  return did_respond() ? AlreadyResponded() : RespondLater();
}
