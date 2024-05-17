DOMHighResTimeStamp GetUnixAtZeroMonotonic() {
  DEFINE_THREAD_SAFE_STATIC_LOCAL(
      DOMHighResTimeStamp, unix_at_zero_monotonic,
      {ConvertSecondsToDOMHighResTimeStamp(CurrentTime() -
                                           CurrentTimeTicksInSeconds())});
  return unix_at_zero_monotonic;
}
