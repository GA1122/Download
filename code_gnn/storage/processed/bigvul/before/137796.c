TabRecordingIndicatorAnimation::Create() {
  MultiAnimation::Parts parts;
  static_assert(kCaptureIndicatorThrobCycles % 2 != 0,
        "odd number of cycles required so animation finishes in showing state");
  for (int i = 0; i < kCaptureIndicatorThrobCycles; ++i) {
    parts.push_back(MultiAnimation::Part(
        i % 2 ? kIndicatorFadeOutDurationMs : kIndicatorFadeInDurationMs,
        gfx::Tween::EASE_IN));
  }
  const base::TimeDelta interval =
      base::TimeDelta::FromMilliseconds(kIndicatorFrameIntervalMs);
  std::unique_ptr<TabRecordingIndicatorAnimation> animation(
      new TabRecordingIndicatorAnimation(parts, interval));
  animation->set_continuous(false);
  return animation;
}
