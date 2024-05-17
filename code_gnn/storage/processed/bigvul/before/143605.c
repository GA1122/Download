inline blink::WebGestureEvent CreateScrollEndForWrapping(
    const blink::WebGestureEvent& gesture_event) {
  DCHECK(gesture_event.type == blink::WebInputEvent::GestureScrollUpdate);

  blink::WebGestureEvent wrap_gesture_scroll_end;
  wrap_gesture_scroll_end.type = blink::WebInputEvent::GestureScrollEnd;
  wrap_gesture_scroll_end.sourceDevice = gesture_event.sourceDevice;
  wrap_gesture_scroll_end.resendingPluginId = gesture_event.resendingPluginId;

  return wrap_gesture_scroll_end;
}
