double LMUvalueToLux(uint64_t raw_value) {

  static const long double k4 = pow(10.L, -7);
  static const long double k3 = pow(10.L, -4);
  static const long double k2 = pow(10.L, -2);
  static const long double k1 = pow(10.L, 5);
  long double scaled_value = raw_value / k1;

  long double lux_value =
      (-3 * k4 * pow(scaled_value, 4)) + (2.6 * k3 * pow(scaled_value, 3)) +
      (-3.4 * k2 * pow(scaled_value, 2)) + (3.9 * scaled_value) - 0.19;

  double lux = ceil(static_cast<double>(lux_value));
  return lux > 0 ? lux : 0;
}
