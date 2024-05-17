void PlatformSensorProviderLinux::OnSensorNodesEnumerated() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(!sensor_nodes_enumerated_);
  sensor_nodes_enumerated_ = true;
  ProcessStoredRequests();
}
