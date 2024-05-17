void RenderWidgetHostImpl::OnTouchEventAck(
    const TouchEventWithLatencyInfo& event,
    InputEventAckSource ack_source,
    InputEventAckState ack_result) {
  latency_tracker_.OnInputEventAck(event.event, &event.latency, ack_result);
  for (auto& input_event_observer : input_event_observers_)
    input_event_observer.OnInputEventAck(ack_source, ack_result, event.event);

  if (touch_emulator_ &&
      touch_emulator_->HandleTouchEventAck(event.event, ack_result)) {
    return;
  }

  if (view_)
    view_->ProcessAckedTouchEvent(event, ack_result);
}
