int ScaleAndRound(int value, double scaling) {
  return static_cast<int>(static_cast<double>(value) / scaling);
}
