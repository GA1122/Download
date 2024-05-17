void GestureSequence::AppendScrollGestureEnd(const GesturePoint& point,
                                             const gfx::Point& location,
                                             Gestures* gestures,
                                             float x_velocity,
                                             float y_velocity) {
  gestures->push_back(linked_ptr<GestureEvent>(new GestureEvent(
      ui::ET_GESTURE_SCROLL_END,
      location.x(),
      location.y(),
      flags_,
      base::Time::FromDoubleT(point.last_touch_time()),
      x_velocity, y_velocity)));
}
