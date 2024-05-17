void RenderWidgetHostImpl::ForwardMouseEventWithLatencyInfo(
    const blink::WebMouseEvent& mouse_event,
    const ui::LatencyInfo& latency) {
  TRACE_EVENT2("input", "RenderWidgetHostImpl::ForwardMouseEvent", "x",
               mouse_event.PositionInWidget().x, "y",
               mouse_event.PositionInWidget().y);

  DCHECK_GE(mouse_event.GetType(), blink::WebInputEvent::kMouseTypeFirst);
  DCHECK_LE(mouse_event.GetType(), blink::WebInputEvent::kMouseTypeLast);

  for (size_t i = 0; i < mouse_event_callbacks_.size(); ++i) {
    if (mouse_event_callbacks_[i].Run(mouse_event))
      return;
  }

  if (ShouldDropInputEvents())
    return;

  auto* touch_emulator = GetExistingTouchEmulator();
  if (touch_emulator &&
      touch_emulator->HandleMouseEvent(mouse_event, GetView())) {
    return;
  }

  MouseEventWithLatencyInfo mouse_with_latency(mouse_event, latency);
  DispatchInputEventWithLatencyInfo(mouse_event, &mouse_with_latency.latency);
  input_router_->SendMouseEvent(mouse_with_latency);
}
