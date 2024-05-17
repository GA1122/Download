bool MockDaemonProcess::OnMessageReceived(const IPC::Message& message) {
  Received(message);

  return DaemonProcess::OnMessageReceived(message);
}
