  void Start() {
    animation_.SetSlideDuration(GetDuration());
    animation_.SetTweenType(ui::Tween::EASE_OUT);
    if (!animation_.IsShowing()) {
      animation_.Reset();
      animation_.Show();
    }
  }
