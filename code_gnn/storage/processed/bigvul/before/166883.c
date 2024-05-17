double Performance::ClampTimeResolution(double time_seconds) {
  DEFINE_THREAD_SAFE_STATIC_LOCAL(TimeClamper, clamper, ());
  return clamper.ClampTimeResolution(time_seconds);
}
