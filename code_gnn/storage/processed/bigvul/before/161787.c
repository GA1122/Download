PlatformSensorLinux::~PlatformSensorLinux() {
  DCHECK(task_runner_->BelongsToCurrentThread());
  polling_thread_task_runner_->DeleteSoon(FROM_HERE, sensor_reader_.release());
}
