SensorInfoLinux* PlatformSensorProviderLinux::GetSensorDevice(
    mojom::SensorType type) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  auto sensor = sensor_devices_by_type_.find(type);
  if (sensor == sensor_devices_by_type_.end())
    return nullptr;
  return sensor->second.get();
}
