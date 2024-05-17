void OverscrollControllerAndroid::OnGestureEventAck(
    const blink::WebGestureEvent& event,
    InputEventAckState ack_result) {
  if (!enabled_)
    return;

  if (event.type == blink::WebInputEvent::GestureScrollEnd ||
      event.type == blink::WebInputEvent::GestureFlingStart) {
    OnOverscrolled(DidOverscrollParams());
  }

  if (event.type == blink::WebInputEvent::GestureScrollUpdate &&
      refresh_effect_) {
    bool consumed = ack_result == INPUT_EVENT_ACK_STATE_CONSUMED ||
                    event.data.scrollUpdate.previousUpdateInSequencePrevented;
    refresh_effect_->OnScrollUpdateAck(consumed);
  }
}
