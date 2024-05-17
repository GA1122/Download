  void CreateAccelerometer() {
    auto callback = base::Bind(&PlatformSensorFusionTest::AccelerometerCallback,
                               base::Unretained(this));
    provider_->CreateSensor(SensorType::ACCELEROMETER, callback);
    EXPECT_TRUE(accelerometer_callback_called_);
    EXPECT_TRUE(accelerometer_);
    EXPECT_EQ(SensorType::ACCELEROMETER, accelerometer_->GetType());
  }
