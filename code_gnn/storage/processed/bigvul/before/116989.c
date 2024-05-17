void GesturePoint::UpdateForScroll() {
  first_touch_position_ = last_touch_position_;
  first_touch_time_ = last_touch_time_;
}
