  PlatformSensorAmbientLightMac::PlatformSensorAmbientLightMac(
    mojo::ScopedSharedBufferMapping mapping,
//     SensorReadingSharedBuffer* reading_buffer,
      PlatformSensorProvider* provider)
    : PlatformSensor(SensorType::AMBIENT_LIGHT, std::move(mapping), provider),
//     : PlatformSensor(SensorType::AMBIENT_LIGHT, reading_buffer, provider),
        light_sensor_port_(nullptr),
        current_lux_(0.0) {}