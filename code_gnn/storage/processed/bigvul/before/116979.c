bool GesturePoint::DidScroll(const TouchEvent& event, int dist) const {
  return abs(last_touch_position_.x() - first_touch_position_.x()) > dist ||
         abs(last_touch_position_.y() - first_touch_position_.y()) > dist;
}
