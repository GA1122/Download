void GestureSequence::AppendScrollGestureUpdate(const GesturePoint& point,
                                                const gfx::Point& location,
                                                Gestures* gestures) {
  int dx = point.x_delta();
  int dy = point.y_delta();

  if (scroll_type_ == ST_HORIZONTAL)
    dy = 0;
  else if (scroll_type_ == ST_VERTICAL)
    dx = 0;

  gestures->push_back(linked_ptr<GestureEvent>(new GestureEvent(
      ui::ET_GESTURE_SCROLL_UPDATE,
      location.x(),
      location.y(),
      flags_,
      base::Time::FromDoubleT(point.last_touch_time()),
      dx, dy)));
}
