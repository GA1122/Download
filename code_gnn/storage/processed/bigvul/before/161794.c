bool PlatformSensorProviderBase::HasSensors() const {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  return !sensor_map_.empty();
}
