bool PlatformSensorLinux::CheckSensorConfiguration(
    const PlatformSensorConfiguration& configuration) {
  DCHECK(task_runner_->BelongsToCurrentThread());
  return configuration.frequency() > 0 &&
         configuration.frequency() <= default_configuration_.frequency();
}
