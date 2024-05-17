void PlatformSensorProviderBase::RemoveSensor(mojom::SensorType type,
                                              PlatformSensor* sensor) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  auto it = sensor_map_.find(type);
  if (it == sensor_map_.end()) {
    return;
  }

  if (sensor != it->second) {
    NOTREACHED()
        << "not expecting to track more than one sensor of the same type";
    return;
  }

  sensor_map_.erase(type);
  FreeResourcesIfNeeded();
}
