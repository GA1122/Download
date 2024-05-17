ExtensionFunction::ResponseAction BluetoothSocketUpdateFunction::Run() {
  auto params = bluetooth_socket::Update::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(params.get());

  BluetoothApiSocket* socket = GetSocket(params->socket_id);
  if (!socket)
    return RespondNow(Error(kSocketNotFoundError));

  SetSocketProperties(socket, &params->properties);
  return RespondNow(ArgumentList(bluetooth_socket::Update::Results::Create()));
}
