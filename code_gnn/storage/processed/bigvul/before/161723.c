MockPlatformSensorClient::~MockPlatformSensorClient() {
  if (sensor_)
    sensor_->RemoveClient(this);
}
