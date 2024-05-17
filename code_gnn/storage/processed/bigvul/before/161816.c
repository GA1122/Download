PlatformSensorProviderWin::CreateSensorReader(mojom::SensorType type) {
  DCHECK(sensor_thread_->task_runner()->BelongsToCurrentThread());
  if (!sensor_thread_->sensor_manager())
    return nullptr;
  return PlatformSensorReaderWin::Create(type,
                                         sensor_thread_->sensor_manager());
}
