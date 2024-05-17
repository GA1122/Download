void PlatformSensorProviderWin::SetSensorManagerForTesting(
    Microsoft::WRL::ComPtr<ISensorManager> sensor_manager) {
  CreateSensorThread();
  sensor_thread_->SetSensorManagerForTesting(sensor_manager);
}
