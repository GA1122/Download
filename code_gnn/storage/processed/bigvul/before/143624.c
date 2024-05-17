void RenderWidgetHostImpl::ForwardMouseEventWithLatencyInfo(
      const blink::WebMouseEvent& mouse_event,
      const ui::LatencyInfo& ui_latency) {
  TRACE_EVENT2("input", "RenderWidgetHostImpl::ForwardMouseEvent",
               "x", mouse_event.x, "y", mouse_event.y);

  for (size_t i = 0; i < mouse_event_callbacks_.size(); ++i) {
    if (mouse_event_callbacks_[i].Run(mouse_event))
      return;
  }

  if (ShouldDropInputEvents())
    return;

  if (touch_emulator_ && touch_emulator_->HandleMouseEvent(mouse_event))
    return;

  MouseEventWithLatencyInfo mouse_with_latency(mouse_event, ui_latency);
  latency_tracker_.OnInputEvent(mouse_event, &mouse_with_latency.latency);
  input_router_->SendMouseEvent(mouse_with_latency);

  if (process_->SubscribeUniformEnabled()) {
    gpu::ValueState state;
    state.int_value[0] = mouse_event.x;
    state.int_value[1] = mouse_event.y;
    process_->SendUpdateValueState(GL_MOUSE_POSITION_CHROMIUM, state);
  }
}
