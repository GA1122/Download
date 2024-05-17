void RenderWidgetHostImpl::DispatchInputEventWithLatencyInfo(
    const blink::WebInputEvent& event,
    ui::LatencyInfo* latency) {
  latency_tracker_.OnInputEvent(event, latency);
  for (auto& observer : input_event_observers_)
    observer.OnInputEvent(event);
}
