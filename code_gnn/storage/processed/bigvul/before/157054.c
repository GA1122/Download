T clamp(T value, T min, T max) {
  return std::max(std::min(value, max), min);
}
