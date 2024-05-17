  base::Optional<WebGestureEvent> GetAndResetLastForwardedGestureEvent() {
    base::Optional<WebGestureEvent> ret;
    last_forwarded_gesture_event_.swap(ret);
    return ret;
  }
