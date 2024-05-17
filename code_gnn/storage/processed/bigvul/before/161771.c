  void AccelerometerCallback(scoped_refptr<PlatformSensor> sensor) {
    accelerometer_callback_called_ = true;
    accelerometer_ = static_cast<FakePlatformSensor*>(sensor.get());
  }
