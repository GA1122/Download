bool PlatformSensorFusion::GetSourceReading(mojom::SensorType type,
                                            SensorReading* result) {
  auto it = source_sensors_.find(type);
  if (it != source_sensors_.end())
    return it->second->GetLatestReading(result);
  NOTREACHED();
  return false;
}
