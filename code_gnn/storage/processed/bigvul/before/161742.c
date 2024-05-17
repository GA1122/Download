void PlatformSensorAccelerometerMac::PollForData() {
  float axis_value[3];
  if (!sudden_motion_sensor_->ReadSensorValues(axis_value))
    return;

  SensorReading reading;
  reading.accel.timestamp =
      (base::TimeTicks::Now() - base::TimeTicks()).InSecondsF();
  reading.accel.x = axis_value[0] * kMeanGravity;
  reading.accel.y = axis_value[1] * kMeanGravity;
  reading.accel.z = axis_value[2] * kMeanGravity;

  if (IsSignificantlyDifferent(reading_, reading)) {
    reading_ = reading;
    UpdateSharedBufferAndNotifyClients(reading);
  }
}
