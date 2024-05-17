bool HaveValuesChanged(const SensorReading& lhs, const SensorReading& rhs) {
  for (size_t i = 0; i < SensorReadingRaw::kValuesCount; ++i) {
    if (lhs.raw.values[i] != rhs.raw.values[i])
      return true;
  }
  return false;
}
