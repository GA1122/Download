ui::GestureStatus UserActivityDetector::PreHandleGestureEvent(
    aura::Window* target,
    aura::GestureEvent* event) {
  MaybeNotify();
  return ui::GESTURE_STATUS_UNKNOWN;
 }
