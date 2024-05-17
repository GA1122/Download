void RenderWidgetHostImpl::OnGestureEventAck(
    const GestureEventWithLatencyInfo& event,
    InputEventAckSource ack_source,
    InputEventAckState ack_result) {
  latency_tracker_.OnInputEventAck(event.event, &event.latency, ack_result);
  for (auto& input_event_observer : input_event_observers_)
    input_event_observer.OnInputEventAck(ack_source, ack_result, event.event);

  if (touch_emulator_)
    touch_emulator_->OnGestureEventAck(event.event);

  if (view_)
    view_->GestureEventAck(event.event, ack_result);
}
