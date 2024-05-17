TouchState TouchEventTypeToTouchState(ui::EventType type) {
  switch (type) {
    case ui::ET_TOUCH_RELEASED:
      return TS_RELEASED;
    case ui::ET_TOUCH_PRESSED:
      return TS_PRESSED;
    case ui::ET_TOUCH_MOVED:
      return TS_MOVED;
    case ui::ET_TOUCH_STATIONARY:
      return TS_STATIONARY;
    case ui::ET_TOUCH_CANCELLED:
      return TS_CANCELLED;
    default:
      VLOG(1) << "Unknown Touch Event type";
  }
  return TS_UNKNOWN;
}
