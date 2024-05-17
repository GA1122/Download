void DaemonProcess::OnConfigUpdated(const std::string& serialized_config) {
  DCHECK(caller_task_runner()->BelongsToCurrentThread());

  if (serialized_config_ != serialized_config) {
    serialized_config_ = serialized_config;
    SendToNetwork(
        new ChromotingDaemonNetworkMsg_Configuration(serialized_config_));
  }
}
