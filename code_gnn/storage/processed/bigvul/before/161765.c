bool PlatformSensorFusion::IsSuspended() {
  for (auto& client : clients_) {
    if (!client.IsSuspended())
      return false;
  }
  return true;
}
