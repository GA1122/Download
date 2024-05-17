bool GestureSequence::PinchEnd(const TouchEvent& event,
    const GesturePoint& point, Gestures* gestures) {
  DCHECK(state_ == GS_PINCH);
  float distance = points_[0].Distance(points_[1]);
  AppendPinchGestureEnd(points_[0], points_[1],
      distance / pinch_distance_start_, gestures);

  pinch_distance_start_ = 0;
  pinch_distance_current_ = 0;
  return true;
}
