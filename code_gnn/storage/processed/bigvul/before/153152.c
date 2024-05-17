void Compositor::FrameIntervalUpdated(base::TimeDelta interval) {
  refresh_rate_ =
      base::Time::kMicrosecondsPerSecond / interval.InMicrosecondsF();
}
