void PlatformSensorProviderLinux::OnDeviceRemoved(
    mojom::SensorType type,
    const std::string& device_node) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  auto it = sensor_devices_by_type_.find(type);
  if (it != sensor_devices_by_type_.end() &&
      it->second->device_node == device_node) {
    sensor_devices_by_type_.erase(it);
  }
}
