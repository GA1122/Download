  PlatformSensor::PlatformSensor(mojom::SensorType type,
                               mojo::ScopedSharedBufferMapping mapping,
//                                SensorReadingSharedBuffer* reading_buffer,
                                 PlatformSensorProvider* provider)
      : task_runner_(base::ThreadTaskRunnerHandle::Get()),
      shared_buffer_mapping_(std::move(mapping)),
//       reading_buffer_(reading_buffer),
        type_(type),
        provider_(provider),
        weak_factory_(this) {}