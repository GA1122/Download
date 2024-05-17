  void MagnetometerCallback(scoped_refptr<PlatformSensor> sensor) {
    magnetometer_callback_called_ = true;
    magnetometer_ = static_cast<FakePlatformSensor*>(sensor.get());
  }
