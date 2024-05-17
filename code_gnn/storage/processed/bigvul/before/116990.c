void GesturePoint::UpdateForTap() {
  last_tap_time_ = last_touch_time_;
  last_tap_position_ = last_touch_position_;
  Reset();
}
