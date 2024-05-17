void OverscrollControllerAndroid::Disable() {
  if (!enabled_)
    return;
  enabled_ = false;
  if (!enabled_) {
    if (refresh_effect_)
      refresh_effect_->Reset();
    if (glow_effect_)
      glow_effect_->Reset();
  }
}
