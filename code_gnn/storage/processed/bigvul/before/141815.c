int64_t MetricsLog::GetBuildTime() {
  static int64_t integral_build_time = 0;
  if (!integral_build_time)
    integral_build_time = static_cast<int64_t>(base::GetBuildTime().ToTimeT());
  return integral_build_time;
}
