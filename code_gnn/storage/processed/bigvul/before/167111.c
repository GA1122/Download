ExtensionFunction::ResponseAction BluetoothSocketCreateFunction::Run() {
  DCHECK_CURRENTLY_ON(work_thread_id());

  auto params = bluetooth_socket::Create::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(params.get());

  BluetoothApiSocket* socket = new BluetoothApiSocket(extension_id());

  bluetooth_socket::SocketProperties* properties = params->properties.get();
  if (properties)
    SetSocketProperties(socket, properties);

  bluetooth_socket::CreateInfo create_info;
  create_info.socket_id = AddSocket(socket);
  return RespondNow(
      ArgumentList(bluetooth_socket::Create::Results::Create(create_info)));
}
