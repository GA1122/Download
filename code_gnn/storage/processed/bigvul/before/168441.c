int PlatformFontSkia::GetExpectedTextWidth(int length) {
  ComputeMetricsIfNecessary();
  return round(static_cast<float>(length) * average_width_pixels_);
}
