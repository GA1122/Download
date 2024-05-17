void PlatformSensorLinux::StopSensor() {
  DCHECK(task_runner_->BelongsToCurrentThread());
  polling_thread_task_runner_->PostTask(
      FROM_HERE, base::Bind(&SensorReader::StopFetchingData,
                            base::Unretained(sensor_reader_.get())));
}
