void RenderWidgetHostViewAura::WheelEventAck(
    const blink::WebMouseWheelEvent& event,
    InputEventAckState ack_result) {
  if (overscroll_controller_) {
    overscroll_controller_->ReceivedEventACK(
        event, (INPUT_EVENT_ACK_STATE_CONSUMED == ack_result));
  }
}
