void RenderWidgetHostImpl::ForwardEmulatedTouchEvent(
      const blink::WebTouchEvent& touch_event) {
  TRACE_EVENT0("input", "RenderWidgetHostImpl::ForwardEmulatedTouchEvent");

  TouchEventWithLatencyInfo touch_with_latency(touch_event);
  latency_tracker_.OnInputEvent(touch_event, &touch_with_latency.latency);
  input_router_->SendTouchEvent(touch_with_latency);
}
