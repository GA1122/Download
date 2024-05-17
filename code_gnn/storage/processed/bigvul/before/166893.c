DOMHighResTimeStamp Performance::MonotonicTimeToDOMHighResTimeStamp(
    TimeTicks time_origin,
    TimeTicks monotonic_time,
    bool allow_negative_value) {
  if (monotonic_time.is_null() || time_origin.is_null())
    return 0.0;

  double clamped_time_in_seconds =
      ClampTimeResolution(TimeTicksInSeconds(monotonic_time)) -
      ClampTimeResolution(TimeTicksInSeconds(time_origin));
  if (clamped_time_in_seconds < 0 && !allow_negative_value)
    return 0.0;
  return ConvertSecondsToDOMHighResTimeStamp(clamped_time_in_seconds);
}
