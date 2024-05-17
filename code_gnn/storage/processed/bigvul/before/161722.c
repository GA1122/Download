MockPlatformSensorClient::MockPlatformSensorClient(
    scoped_refptr<PlatformSensor> sensor)
    : MockPlatformSensorClient() {
  DCHECK(sensor);
  sensor_ = std::move(sensor);
  sensor_->AddClient(this);
}
