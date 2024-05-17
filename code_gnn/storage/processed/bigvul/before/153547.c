  void StopAnimation(Tab* tab) {
    ASSERT_TRUE(tab->alert_indicator_->fade_animation_);
    tab->alert_indicator_->fade_animation_->Stop();
  }
