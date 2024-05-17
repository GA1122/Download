  void PlatformSensorFusionCallback(scoped_refptr<PlatformSensor> sensor) {
    platform_sensor_fusion_callback_called_ = true;
    fusion_sensor_ = static_cast<PlatformSensorFusion*>(sensor.get());
  }
