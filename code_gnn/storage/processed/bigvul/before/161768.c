bool PlatformSensorFusion::StartSensor(
    const PlatformSensorConfiguration& configuration) {
  StopSensor();
  for (const auto& pair : source_sensors_) {
    if (!pair.second->StartListening(this, configuration)) {
      StopSensor();
      return false;
    }
  }

  fusion_algorithm_->SetFrequency(configuration.frequency());
  return true;
}
