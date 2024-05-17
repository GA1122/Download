 mojo::ScopedSharedBufferMapping FakePlatformSensorProvider::GetMapping(
// SensorReadingSharedBuffer* FakePlatformSensorProvider::GetSensorReadingBuffer(
      mojom::SensorType type) {
  return CreateSharedBufferIfNeeded() ? MapSharedBufferForType(type) : nullptr;
//   return CreateSharedBufferIfNeeded()
//              ? GetSensorReadingSharedBufferForType(type)
//              : nullptr;
  }