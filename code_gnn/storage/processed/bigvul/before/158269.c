void RenderWidgetHostImpl::ForwardKeyboardEventWithLatencyInfo(
    const NativeWebKeyboardEvent& key_event,
    const ui::LatencyInfo& latency) {
  ForwardKeyboardEventWithCommands(key_event, latency, nullptr, nullptr);
}
