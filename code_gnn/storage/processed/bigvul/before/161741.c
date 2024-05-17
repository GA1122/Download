bool IsSignificantlyDifferent(const device::SensorReading& reading1,
                              const device::SensorReading& reading2) {
  return (std::fabs(reading1.accel.x - reading2.accel.x) >=
          kGravityThreshold) ||
         (std::fabs(reading1.accel.y - reading2.accel.y) >=
          kGravityThreshold) ||
         (std::fabs(reading1.accel.z - reading2.accel.z) >= kGravityThreshold);
}
