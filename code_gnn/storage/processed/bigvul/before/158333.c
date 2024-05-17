void RenderWidgetHostImpl::OnTouchEventAck(
    const TouchEventWithLatencyInfo& event,
    InputEventAckSource ack_source,
    InputEventAckState ack_result) {
  latency_tracker_.OnInputEventAck(event.event, &event.latency, ack_result);
  for (auto& input_event_observer : input_event_observers_)
    input_event_observer.OnInputEventAck(ack_source, ack_result, event.event);

  auto* input_event_router =
      delegate() ? delegate()->GetInputEventRouter() : nullptr;

  if (input_event_router)
    input_event_router->ProcessAckedTouchEvent(event, ack_result, view_.get());
  else if (view_)
    view_->ProcessAckedTouchEvent(event, ack_result);
}
