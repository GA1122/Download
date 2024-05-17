void MockDaemonProcess::SendToNetwork(IPC::Message* message) {
  Sent(*message);
  delete message;
}
