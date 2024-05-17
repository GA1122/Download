int PlatformFontSkia::GetBaseline() {
  ComputeMetricsIfNecessary();
  return ascent_pixels_;
}
