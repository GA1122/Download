inline blink::WebGestureEvent CreateScrollBeginForWrapping(
    const blink::WebGestureEvent& gesture_event) {
  DCHECK(gesture_event.GetType() == blink::WebInputEvent::kGestureScrollUpdate);

  blink::WebGestureEvent wrap_gesture_scroll_begin(
      blink::WebInputEvent::kGestureScrollBegin, gesture_event.GetModifiers(),
      gesture_event.TimeStamp(), gesture_event.SourceDevice());
  wrap_gesture_scroll_begin.data.scroll_begin.delta_x_hint = 0;
  wrap_gesture_scroll_begin.data.scroll_begin.delta_y_hint = 0;
  wrap_gesture_scroll_begin.resending_plugin_id =
      gesture_event.resending_plugin_id;
  wrap_gesture_scroll_begin.data.scroll_begin.delta_hint_units =
      gesture_event.data.scroll_update.delta_units;

  return wrap_gesture_scroll_begin;
}
