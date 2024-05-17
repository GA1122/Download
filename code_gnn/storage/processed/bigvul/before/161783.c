void PlatformSensorLinux::NotifyPlatformSensorError() {
  DCHECK(task_runner_->BelongsToCurrentThread());
  NotifySensorError();
}
