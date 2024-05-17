void CustomButton::StopThrobbing() {
  if (hover_animation_->is_animating()) {
    hover_animation_->Stop();
    SchedulePaint();
  }
}
