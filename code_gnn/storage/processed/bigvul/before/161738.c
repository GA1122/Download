bool PlatformSensorAccelerometerMac::CheckSensorConfiguration(
    const PlatformSensorConfiguration& configuration) {
  return configuration.frequency() > 0 &&
         configuration.frequency() <=
             SensorTraits<SensorType::ACCELEROMETER>::kMaxAllowedFrequency;
}
