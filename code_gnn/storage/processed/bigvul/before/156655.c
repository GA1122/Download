void GenerateTapDownGesture(RenderWidgetHost* rwh) {
  blink::WebGestureEvent gesture_tap_down(
      blink::WebGestureEvent::kGestureTapDown,
      blink::WebInputEvent::kNoModifiers,
      blink::WebInputEvent::GetStaticTimeStampForTests(),
      blink::kWebGestureDeviceTouchscreen);
  gesture_tap_down.is_source_touch_event_set_non_blocking = true;
  rwh->ForwardGestureEvent(gesture_tap_down);
}
