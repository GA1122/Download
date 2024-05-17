  void FakePlatformSensorProvider::CreateSensorInternal(
      mojom::SensorType type,
    mojo::ScopedSharedBufferMapping mapping,
//     SensorReadingSharedBuffer* reading_buffer,
      const CreateSensorCallback& callback) {
    DCHECK(type >= mojom::SensorType::FIRST && type <= mojom::SensorType::LAST);
    auto sensor =
      base::MakeRefCounted<FakePlatformSensor>(type, std::move(mapping), this);
//       base::MakeRefCounted<FakePlatformSensor>(type, reading_buffer, this);
    DoCreateSensorInternal(type, std::move(sensor), callback);
  }