bool PlatformSensorWin::StartSensor(
    const PlatformSensorConfiguration& configuration) {
  DCHECK(task_runner_->BelongsToCurrentThread());
  return sensor_reader_->StartSensor(configuration);
}
