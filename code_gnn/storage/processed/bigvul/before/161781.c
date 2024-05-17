mojom::ReportingMode PlatformSensorLinux::GetReportingMode() {
  DCHECK(task_runner_->BelongsToCurrentThread());
  return reporting_mode_;
}
