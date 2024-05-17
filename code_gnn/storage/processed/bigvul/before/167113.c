ExtensionFunction::ResponseAction BluetoothSocketSetPausedFunction::Run() {
  auto params = bluetooth_socket::SetPaused::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(params.get());

  BluetoothSocketEventDispatcher* socket_event_dispatcher =
      GetSocketEventDispatcher(browser_context());
  if (!socket_event_dispatcher)
    return RespondNow(Error("Could not get socket event dispatcher."));

  BluetoothApiSocket* socket = GetSocket(params->socket_id);
  if (!socket)
    return RespondNow(Error(kSocketNotFoundError));

  if (socket->paused() != params->paused) {
    socket->set_paused(params->paused);
    if (!params->paused) {
      socket_event_dispatcher->OnSocketResume(extension_id(),
                                              params->socket_id);
    }
  }

  return RespondNow(
      ArgumentList(bluetooth_socket::SetPaused::Results::Create()));
}
