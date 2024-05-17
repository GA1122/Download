bool PlatformSensorAmbientLightMac::CheckSensorConfiguration(
    const PlatformSensorConfiguration& configuration) {
  return configuration.frequency() > 0 &&
         configuration.frequency() <=
             SensorTraits<SensorType::AMBIENT_LIGHT>::kMaxAllowedFrequency;
}
