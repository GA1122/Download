void PlatformSensorLinux::UpdatePlatformSensorReading(SensorReading reading) {
  DCHECK(task_runner_->BelongsToCurrentThread());
  if (GetReportingMode() == mojom::ReportingMode::ON_CHANGE &&
      !HaveValuesChanged(reading, old_values_)) {
    return;
  }
  old_values_ = reading;
  reading.raw.timestamp =
      (base::TimeTicks::Now() - base::TimeTicks()).InSecondsF();
  UpdateSharedBufferAndNotifyClients(reading);
}
