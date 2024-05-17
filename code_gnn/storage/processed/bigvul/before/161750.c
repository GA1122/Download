void PlatformSensorAmbientLightMac::StopSensor() {
  light_sensor_port_.reset();
  light_sensor_notification_.reset();
  light_sensor_busy_notification_.reset();
  light_sensor_object_.reset();
  light_sensor_service_.reset();
  current_lux_ = 0.0;
}
