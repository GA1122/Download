 void PlatformSensorProviderLinux::CreateSensorAndNotify(
     mojom::SensorType type,
      SensorInfoLinux* sensor_device) {
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
    scoped_refptr<PlatformSensorLinux> sensor;
  mojo::ScopedSharedBufferMapping mapping = MapSharedBufferForType(type);
  if (sensor_device && mapping && StartPollingThread()) {
    sensor =
        new PlatformSensorLinux(type, std::move(mapping), this, sensor_device,
                                polling_thread_->task_runner());
//   SensorReadingSharedBuffer* reading_buffer =
//       GetSensorReadingSharedBufferForType(type);
//   if (sensor_device && reading_buffer && StartPollingThread()) {
//     sensor = new PlatformSensorLinux(type, reading_buffer, this, sensor_device,
//                                      polling_thread_->task_runner());
    }
    NotifySensorCreated(type, sensor);
  }