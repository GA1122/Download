void RenderWidgetHostViewAura::ProcessTouchEvent(
    const blink::WebTouchEvent& event,
    const ui::LatencyInfo& latency) {
  host_->ForwardTouchEventWithLatencyInfo(event, latency);
}
