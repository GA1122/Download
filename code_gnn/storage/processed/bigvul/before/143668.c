void RenderWidgetHostImpl::OnMouseEventAck(
    const MouseEventWithLatencyInfo& mouse_event,
    InputEventAckState ack_result) {
  latency_tracker_.OnInputEventAck(mouse_event.event, &mouse_event.latency);
}
