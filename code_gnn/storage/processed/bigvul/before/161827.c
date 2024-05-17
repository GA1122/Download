double PlatformSensorWin::GetMaximumSupportedFrequency() {
  double minimal_reporting_interval_ms =
      sensor_reader_->GetMinimalReportingIntervalMs();
  if (!minimal_reporting_interval_ms)
    return kDefaultSensorReportingFrequency;
  return base::Time::kMillisecondsPerSecond / minimal_reporting_interval_ms;
}
