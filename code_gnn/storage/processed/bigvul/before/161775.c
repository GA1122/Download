  void CreateMagnetometer() {
    auto callback = base::Bind(&PlatformSensorFusionTest::MagnetometerCallback,
                               base::Unretained(this));
    provider_->CreateSensor(SensorType::MAGNETOMETER, callback);
    EXPECT_TRUE(magnetometer_callback_called_);
    EXPECT_TRUE(magnetometer_);
    EXPECT_EQ(SensorType::MAGNETOMETER, magnetometer_->GetType());
  }
