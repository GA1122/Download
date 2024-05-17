float GesturePoint::Distance(const GesturePoint& point) const {
  float x_diff = point.last_touch_position_.x() - last_touch_position_.x();
  float y_diff = point.last_touch_position_.y() - last_touch_position_.y();
  return sqrt(x_diff * x_diff + y_diff * y_diff);
}
