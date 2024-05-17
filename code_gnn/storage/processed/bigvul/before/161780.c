PlatformSensorConfiguration PlatformSensorLinux::GetDefaultConfiguration() {
  DCHECK(task_runner_->BelongsToCurrentThread());
  return default_configuration_;
}
