ExtensionFunction::ResponseAction BluetoothSocketGetInfoFunction::Run() {
  auto params = bluetooth_socket::GetInfo::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(params.get());

  BluetoothApiSocket* socket = GetSocket(params->socket_id);
  if (!socket)
    return RespondNow(Error(kSocketNotFoundError));

  return RespondNow(ArgumentList(bluetooth_socket::GetInfo::Results::Create(
      CreateSocketInfo(params->socket_id, socket))));
}
