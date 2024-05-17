void PlatformSensorWin::StopSensor() {
  DCHECK(task_runner_->BelongsToCurrentThread());
  sensor_reader_->StopSensor();
}
