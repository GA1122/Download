scoped_refptr<PlatformSensor> PlatformSensorProviderBase::GetSensor(
    mojom::SensorType type) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

  auto it = sensor_map_.find(type);
  if (it != sensor_map_.end())
    return it->second;
  return nullptr;
}
