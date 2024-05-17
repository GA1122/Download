  PlatformSensorAccelerometerMac::PlatformSensorAccelerometerMac(
    mojo::ScopedSharedBufferMapping mapping,
//     SensorReadingSharedBuffer* reading_buffer,
      PlatformSensorProvider* provider)
    : PlatformSensor(SensorType::ACCELEROMETER, std::move(mapping), provider),
//     : PlatformSensor(SensorType::ACCELEROMETER, reading_buffer, provider),
        sudden_motion_sensor_(SuddenMotionSensor::Create()) {}