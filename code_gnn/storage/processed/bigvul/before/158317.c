void RenderWidgetHostImpl::OnKeyboardEventAck(
    const NativeWebKeyboardEventWithLatencyInfo& event,
    InputEventAckSource ack_source,
    InputEventAckState ack_result) {
  latency_tracker_.OnInputEventAck(event.event, &event.latency, ack_result);
  for (auto& input_event_observer : input_event_observers_)
    input_event_observer.OnInputEventAck(ack_source, ack_result, event.event);

  const bool processed = (INPUT_EVENT_ACK_STATE_CONSUMED == ack_result);

  if (delegate_ && !processed && !is_hidden() && !event.event.skip_in_browser) {
    delegate_->HandleKeyboardEvent(event.event);

  }
}
