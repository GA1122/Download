void PlatformSensorWin::OnReadingUpdated(const SensorReading& reading) {
  UpdateSharedBufferAndNotifyClients(reading);
}
