static WebGestureEvent MakeGestureEvent(WebInputEvent::Type type,
                                        double timestamp_seconds,
                                        int x,
                                        int y,
                                        int modifiers) {
  WebGestureEvent result;

  result.type = type;
  result.x = x;
  result.y = y;
  result.timeStampSeconds = timestamp_seconds;
  result.modifiers = modifiers;

  return result;
}
