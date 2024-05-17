void SettingLevelBubble::StopAnimation() {
  animation_timer_.Stop();
  is_animating_ = false;
}
