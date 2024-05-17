void RenderWidgetHostImpl::ForwardTouchEventWithLatencyInfo(
    const blink::WebTouchEvent& touch_event,
    const ui::LatencyInfo& latency) {
  TRACE_EVENT0("input", "RenderWidgetHostImpl::ForwardTouchEvent");


  TouchEventWithLatencyInfo touch_with_latency(touch_event, latency);
  DispatchInputEventWithLatencyInfo(touch_event, &touch_with_latency.latency);
  input_router_->SendTouchEvent(touch_with_latency);
}
