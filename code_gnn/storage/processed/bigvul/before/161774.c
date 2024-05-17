  void CreateLinearAccelerationFusionSensor() {
    auto fusion_algorithm =
        std::make_unique<LinearAccelerationFusionAlgorithmUsingAccelerometer>();
    CreateFusionSensor(std::move(fusion_algorithm));
  }
