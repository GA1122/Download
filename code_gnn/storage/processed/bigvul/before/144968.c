void RenderWidgetHostViewAura::ProcessMouseWheelEvent(
    const blink::WebMouseWheelEvent& event,
    const ui::LatencyInfo& latency) {
  host_->ForwardWheelEventWithLatencyInfo(event, latency);
}
