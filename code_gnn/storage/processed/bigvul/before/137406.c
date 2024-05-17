void RenderViewTest::SendWebGestureEvent(
    const blink::WebGestureEvent& gesture_event) {
  SendInputEvent(gesture_event);
}
