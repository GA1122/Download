void FakeBluetoothAgentManagerClient::RequestDefaultAgent(
    const dbus::ObjectPath& agent_path,
    const base::Closure& callback,
    const ErrorCallback& error_callback) {
  VLOG(1) << "RequestDefaultAgent: " << agent_path.value();
  callback.Run();
}
