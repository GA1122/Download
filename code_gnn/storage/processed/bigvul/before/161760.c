bool PlatformSensorFusion::CheckSensorConfiguration(
    const PlatformSensorConfiguration& configuration) {
  for (const auto& pair : source_sensors_) {
    if (!pair.second->CheckSensorConfiguration(configuration))
      return false;
  }
  return true;
}
