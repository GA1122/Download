ExtensionFunction::ResponseAction BluetoothSocketCloseFunction::Run() {
  auto params = bluetooth_socket::Close::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(params.get());
  BluetoothApiSocket* socket = GetSocket(params->socket_id);
  if (!socket)
    return RespondNow(Error(kSocketNotFoundError));

  RemoveSocket(params->socket_id);
  return RespondNow(ArgumentList(bluetooth_socket::Close::Results::Create()));
}
