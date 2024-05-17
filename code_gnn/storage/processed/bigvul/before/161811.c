void PlatformSensorProviderLinux::Shutdown() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  const bool did_post_task = file_task_runner_->DeleteSoon(
      FROM_HERE, sensor_device_manager_.release());
  DCHECK(did_post_task);
  sensor_nodes_enumerated_ = false;
  sensor_nodes_enumeration_started_ = false;
  sensor_devices_by_type_.clear();
}
