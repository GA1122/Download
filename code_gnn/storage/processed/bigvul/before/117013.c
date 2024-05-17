bool GestureSequence::TouchDown(const TouchEvent& event,
    const GesturePoint& point, Gestures* gestures) {
  DCHECK(state_ == GS_NO_GESTURE);
  AppendTapDownGestureEvent(point, gestures);
  return true;
}
