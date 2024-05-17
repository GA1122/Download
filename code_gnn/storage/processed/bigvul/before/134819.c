float TouchEventConverterEvdev::ScalePressure(int32_t value) {
  float pressure = value - pressure_min_;
  if (pressure_max_ - pressure_min_)
    pressure /= pressure_max_ - pressure_min_;
  return pressure;
}
