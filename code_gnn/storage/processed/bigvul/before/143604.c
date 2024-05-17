inline blink::WebGestureEvent CreateScrollBeginForWrapping(
    const blink::WebGestureEvent& gesture_event) {
  DCHECK(gesture_event.type == blink::WebInputEvent::GestureScrollUpdate);

  blink::WebGestureEvent wrap_gesture_scroll_begin;
  wrap_gesture_scroll_begin.type = blink::WebInputEvent::GestureScrollBegin;
  wrap_gesture_scroll_begin.sourceDevice = gesture_event.sourceDevice;
  wrap_gesture_scroll_begin.data.scrollBegin.deltaXHint = 0;
  wrap_gesture_scroll_begin.data.scrollBegin.deltaYHint = 0;
  wrap_gesture_scroll_begin.resendingPluginId = gesture_event.resendingPluginId;

  return wrap_gesture_scroll_begin;
}
