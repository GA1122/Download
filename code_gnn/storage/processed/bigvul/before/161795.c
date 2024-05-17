void PlatformSensorProviderBase::NotifySensorCreated(
    mojom::SensorType type,
    scoped_refptr<PlatformSensor> sensor) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(!ContainsKey(sensor_map_, type));
  DCHECK(ContainsKey(requests_map_, type));

  if (sensor)
    sensor_map_[type] = sensor.get();

  auto it = requests_map_.find(type);
  CallbackQueue callback_queue = it->second;
  requests_map_.erase(type);

  FreeResourcesIfNeeded();

  for (auto& callback : callback_queue)
    callback.Run(sensor);
}
