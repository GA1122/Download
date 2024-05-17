void RenderWidgetHostImpl::ForwardKeyboardEvent(
    const NativeWebKeyboardEvent& key_event) {
  ui::LatencyInfo latency_info;

  if (key_event.GetType() == WebInputEvent::kRawKeyDown ||
      key_event.GetType() == WebInputEvent::kChar) {
    latency_info.set_source_event_type(ui::SourceEventType::KEY_PRESS);
  }
  ForwardKeyboardEventWithLatencyInfo(key_event, latency_info);
}
