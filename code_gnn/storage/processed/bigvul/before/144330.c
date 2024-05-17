void LoginDisplayHostWebUI::ScheduleFadeOutAnimation(int animation_speed_ms) {
  if (!login_window_) {
    ShutdownDisplayHost();
    return;
  }
  ui::Layer* layer = login_window_->GetLayer();
  ui::ScopedLayerAnimationSettings animation(layer->GetAnimator());
  animation.AddObserver(new AnimationObserver(
      base::Bind(&LoginDisplayHostWebUI::ShutdownDisplayHost,
                 weak_factory_.GetWeakPtr())));
  animation.SetTransitionDuration(
      base::TimeDelta::FromMilliseconds(animation_speed_ms));
  layer->SetOpacity(0);
}
