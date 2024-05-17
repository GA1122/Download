InputEventAckState RenderWidgetHostViewAura::FilterInputEvent(
    const blink::WebInputEvent& input_event) {
  bool consumed = false;
  if (input_event.GetType() == WebInputEvent::kGestureFlingStart) {
    const WebGestureEvent& gesture_event =
        static_cast<const WebGestureEvent&>(input_event);
    if (gesture_event.source_device == blink::kWebGestureDeviceTouchpad &&
        !gesture_event.data.fling_start.velocity_x &&
        !gesture_event.data.fling_start.velocity_y) {
      consumed = true;
    }
  }

  if (overscroll_controller_)
    consumed |= overscroll_controller_->WillHandleEvent(input_event);

  if (WebTouchEvent::IsTouchEventType(input_event.GetType()))
    return INPUT_EVENT_ACK_STATE_NOT_CONSUMED;

  if (consumed &&
      input_event.GetType() == blink::WebInputEvent::kGestureFlingStart) {
    return INPUT_EVENT_ACK_STATE_NO_CONSUMER_EXISTS;
  }

  return consumed ? INPUT_EVENT_ACK_STATE_CONSUMED
                  : INPUT_EVENT_ACK_STATE_NOT_CONSUMED;
}
