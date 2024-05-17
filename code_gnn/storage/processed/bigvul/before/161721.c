MockPlatformSensorClient::MockPlatformSensorClient() {
  ON_CALL(*this, IsSuspended()).WillByDefault(Return(false));
}
