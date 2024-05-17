bool PlatformSensorAmbientLightMac::ReadAndUpdate() {
  uint32_t scalar_output_count = 2;
  uint64_t lux_values[2];
  kern_return_t kr = IOConnectCallMethod(
      light_sensor_object_, LmuFunctionIndex::kGetSensorReadingID, nullptr, 0,
      nullptr, 0, lux_values, &scalar_output_count, nullptr, 0);

  if (kr != KERN_SUCCESS)
    return false;

  uint64_t mean = (lux_values[0] + lux_values[1]) / 2;
  double lux = LMUvalueToLux(mean);
  if (lux == current_lux_)
    return true;
  current_lux_ = lux;

  SensorReading reading;
  reading.als.timestamp =
      (base::TimeTicks::Now() - base::TimeTicks()).InSecondsF();
  reading.als.value = current_lux_;
  UpdateSharedBufferAndNotifyClients(reading);
  return true;
}
