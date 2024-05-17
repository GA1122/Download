void RenderWidgetHostImpl::ForwardWheelEventWithLatencyInfo(
      const blink::WebMouseWheelEvent& wheel_event,
      const ui::LatencyInfo& ui_latency) {
  TRACE_EVENT2("input", "RenderWidgetHostImpl::ForwardWheelEvent",
               "dx", wheel_event.deltaX, "dy", wheel_event.deltaY);

  if (ShouldDropInputEvents())
    return;

  if (touch_emulator_ && touch_emulator_->HandleMouseWheelEvent(wheel_event))
    return;

  MouseWheelEventWithLatencyInfo wheel_with_latency(wheel_event, ui_latency);
  latency_tracker_.OnInputEvent(wheel_event, &wheel_with_latency.latency);
  input_router_->SendWheelEvent(wheel_with_latency);
}
