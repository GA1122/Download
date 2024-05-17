  void SetSensorManagerForTesting(
      Microsoft::WRL::ComPtr<ISensorManager> sensor_manager) {
    sensor_manager_ = sensor_manager;
  }
