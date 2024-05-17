void RenderWidgetHostImpl::OnWheelEventAck(
    const MouseWheelEventWithLatencyInfo& wheel_event,
    InputEventAckState ack_result) {
  latency_tracker_.OnInputEventAck(wheel_event.event, &wheel_event.latency);

  if (!is_hidden() && view_) {
    if (ack_result != INPUT_EVENT_ACK_STATE_CONSUMED &&
        delegate_ && delegate_->HandleWheelEvent(wheel_event.event)) {
      ack_result = INPUT_EVENT_ACK_STATE_CONSUMED;
    }
    view_->WheelEventAck(wheel_event.event, ack_result);
  }
}
