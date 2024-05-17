void StopDiscoverySession(
    std::unique_ptr<device::BluetoothDiscoverySession> discovery_session) {
  discovery_session->Stop(base::DoNothing(), base::DoNothing());
}
