void RenderWidgetHostImpl::ForwardTouchEventWithLatencyInfo(
    const blink::WebTouchEvent& touch_event,
    const ui::LatencyInfo& latency) {
  TRACE_EVENT0("input", "RenderWidgetHostImpl::ForwardTouchEvent");


  TouchEventWithLatencyInfo touch_with_latency(touch_event, latency);
  if (touch_emulator_ &&
      touch_emulator_->HandleTouchEvent(touch_with_latency.event)) {
    if (view_) {
      view_->ProcessAckedTouchEvent(
          touch_with_latency, INPUT_EVENT_ACK_STATE_CONSUMED);
    }
    return;
  }

  DispatchInputEventWithLatencyInfo(touch_event, &touch_with_latency.latency);
  input_router_->SendTouchEvent(touch_with_latency);
}
