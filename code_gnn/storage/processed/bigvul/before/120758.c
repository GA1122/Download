void FakeBluetoothAgentManagerClient::UnregisterAgentServiceProvider(
    FakeBluetoothAgentServiceProvider* service_provider) {
  if (service_provider_ == service_provider)
    service_provider_ = NULL;
}
