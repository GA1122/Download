void GestureSequence::AppendPinchGestureBegin(const GesturePoint& p1,
                                              const GesturePoint& p2,
                                              Gestures* gestures) {
  gfx::Point center = p1.last_touch_position().Middle(p2.last_touch_position());
  gestures->push_back(linked_ptr<GestureEvent>(new GestureEvent(
      ui::ET_GESTURE_PINCH_BEGIN,
      center.x(),
      center.y(),
      flags_,
      base::Time::FromDoubleT(p1.last_touch_time()),
      0.f, 0.f)));
}
