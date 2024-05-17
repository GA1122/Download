void PlatformSensorFusion::OnSensorReadingChanged(mojom::SensorType type) {
  SensorReading reading;
  reading.raw.timestamp =
      (base::TimeTicks::Now() - base::TimeTicks()).InSecondsF();

  if (!fusion_algorithm_->GetFusedData(type, &reading))
    return;

  if (GetReportingMode() == mojom::ReportingMode::ON_CHANGE &&
      !fusion_algorithm_->IsReadingSignificantlyDifferent(reading_, reading)) {
    return;
  }

  reading_ = reading;
  UpdateSharedBufferAndNotifyClients(reading_);
}
