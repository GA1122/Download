void GestureSequence::AppendScrollGestureBegin(const GesturePoint& point,
                                               const gfx::Point& location,
                                               Gestures* gestures) {
  gestures->push_back(linked_ptr<GestureEvent>(new GestureEvent(
      ui::ET_GESTURE_SCROLL_BEGIN,
      location.x(),
      location.y(),
      flags_,
      base::Time::FromDoubleT(point.last_touch_time()),
      0.f, 0.f)));
}
