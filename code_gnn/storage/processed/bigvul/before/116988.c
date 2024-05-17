void GesturePoint::Reset() {
  first_touch_time_ = last_touch_time_ = 0.0;
  velocity_calculator_.ClearHistory();
}
