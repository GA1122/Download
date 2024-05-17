void FakeBluetoothAgentManagerClient::RegisterAgent(
    const dbus::ObjectPath& agent_path,
    const std::string& capability,
    const base::Closure& callback,
    const ErrorCallback& error_callback) {
  VLOG(1) << "RegisterAgent: " << agent_path.value();

  if (service_provider_ == NULL) {
    error_callback.Run(bluetooth_agent_manager::kErrorInvalidArguments,
                       "No agent created");
  } else if (service_provider_->object_path_ != agent_path) {
    error_callback.Run(bluetooth_agent_manager::kErrorAlreadyExists,
                       "Agent already registered");
  } else {
    callback.Run();
  }
}
