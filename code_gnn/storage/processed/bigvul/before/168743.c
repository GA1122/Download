void RenderWidgetHostImpl::ForwardEmulatedTouchEvent(
      const blink::WebTouchEvent& touch_event) {
  TRACE_EVENT0("input", "RenderWidgetHostImpl::ForwardEmulatedTouchEvent");
  ui::LatencyInfo latency_info(ui::SourceEventType::TOUCH);
  TouchEventWithLatencyInfo touch_with_latency(touch_event, latency_info);
  DispatchInputEventWithLatencyInfo(touch_event, &touch_with_latency.latency);
  input_router_->SendTouchEvent(touch_with_latency);
}
