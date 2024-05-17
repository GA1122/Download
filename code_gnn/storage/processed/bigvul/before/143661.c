void RenderWidgetHostImpl::OnGestureEventAck(
    const GestureEventWithLatencyInfo& event,
    InputEventAckState ack_result) {
  latency_tracker_.OnInputEventAck(event.event, &event.latency);

  if (view_)
    view_->GestureEventAck(event.event, ack_result);
}
