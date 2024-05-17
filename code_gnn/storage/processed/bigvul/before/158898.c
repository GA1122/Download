int CalculateCenterForZoom(int center, int length, double zoom) {
  int adjusted_center =
      static_cast<int>(center * zoom) - static_cast<int>(length * zoom / 2);
  return std::max(adjusted_center, 0);
}
