bool PlatformSensorProviderWin::StartSensorThread() {
  CreateSensorThread();
  if (!sensor_thread_->IsRunning())
    return sensor_thread_->Start();
  return true;
}
