  static void StopFadeAnimationIfNecessary(const Tab& tab) {
    gfx::Animation* fade_animation =
        tab.alert_indicator_->fade_animation_.get();
    if (fade_animation)
      fade_animation->Stop();
  }
