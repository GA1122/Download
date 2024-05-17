bool PlatformSensorLinux::StartSensor(
    const PlatformSensorConfiguration& configuration) {
  DCHECK(task_runner_->BelongsToCurrentThread());
  polling_thread_task_runner_->PostTask(
      FROM_HERE,
      base::Bind(&SensorReader::StartFetchingData,
                 base::Unretained(sensor_reader_.get()), configuration));
  return true;
}
