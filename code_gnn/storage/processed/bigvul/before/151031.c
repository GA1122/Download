void DevToolsUIBindings::SendPortForwardingStatus(const base::Value& status) {
  CallClientFunction("DevToolsAPI.devicesPortForwardingStatusChanged", &status,
                     nullptr, nullptr);
}
