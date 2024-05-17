void SettingLevelBubble::OnAnimationTimeout() {
  const TimeTicks now = TimeTicks::Now();
  const int64 remaining_ms = (target_time_ - now).InMilliseconds();

  if (remaining_ms <= 0) {
    current_percent_ = target_percent_;
    StopAnimation();
  } else {
    const double remaining_percent = target_percent_ - current_percent_;
    const int64 elapsed_ms =
        (now - last_animation_update_time_).InMilliseconds();
    current_percent_ +=
        remaining_percent *
        (static_cast<double>(elapsed_ms) / (elapsed_ms + remaining_ms));
  }
  last_animation_update_time_ = now;

  if (view_)
    view_->SetLevel(current_percent_);
}
