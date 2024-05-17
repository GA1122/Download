std::unique_ptr<gfx::Animation> CreateTabAlertIndicatorFadeAnimation(
    TabAlertState alert_state) {
  if (alert_state == TabAlertState::MEDIA_RECORDING ||
      alert_state == TabAlertState::TAB_CAPTURING) {
    return TabRecordingIndicatorAnimation::Create();
  }

  gfx::MultiAnimation::Parts parts;
  const bool is_for_fade_in = (alert_state != TabAlertState::NONE);
  parts.push_back(gfx::MultiAnimation::Part(
      is_for_fade_in ? kIndicatorFadeInDurationMs : kIndicatorFadeOutDurationMs,
      gfx::Tween::EASE_IN));
  const base::TimeDelta interval =
      base::TimeDelta::FromMilliseconds(kIndicatorFrameIntervalMs);
  std::unique_ptr<gfx::MultiAnimation> animation(
      new gfx::MultiAnimation(parts, interval));
  animation->set_continuous(false);
  return std::move(animation);
}
