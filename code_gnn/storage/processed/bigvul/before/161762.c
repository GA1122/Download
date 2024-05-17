PlatformSensorConfiguration PlatformSensorFusion::GetDefaultConfiguration() {
  PlatformSensorConfiguration default_configuration;
  for (const auto& pair : source_sensors_) {
    double frequency = pair.second->GetDefaultConfiguration().frequency();
    if (frequency > default_configuration.frequency())
      default_configuration.set_frequency(frequency);
  }
  return default_configuration;
}
