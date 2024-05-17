void RenderWidgetHostImpl::ForwardKeyboardEvent(
    const NativeWebKeyboardEvent& key_event) {
  TRACE_EVENT0("input", "RenderWidgetHostImpl::ForwardKeyboardEvent");
  if (owner_delegate_ &&
      !owner_delegate_->MayRenderWidgetForwardKeyboardEvent(key_event)) {
    return;
  }

  if (ShouldDropInputEvents())
    return;

  if (!process_->HasConnection())
    return;

  if (KeyPressListenersHandleEvent(key_event)) {
    if (key_event.type == WebKeyboardEvent::RawKeyDown)
      suppress_next_char_events_ = true;
    return;
  }

  if (!WebInputEvent::isKeyboardEventType(key_event.type))
    return;

  if (suppress_next_char_events_) {
    if (key_event.type == WebKeyboardEvent::Char)
      return;
    suppress_next_char_events_ = false;
  }

  bool is_shortcut = false;

  if (delegate_ && !key_event.skip_in_browser) {
    if (key_event.type == WebKeyboardEvent::RawKeyDown)
      suppress_next_char_events_ = true;

    if (delegate_->PreHandleKeyboardEvent(key_event, &is_shortcut))
      return;

    if (key_event.type == WebKeyboardEvent::RawKeyDown)
      suppress_next_char_events_ = false;
  }

  if (touch_emulator_ && touch_emulator_->HandleKeyboardEvent(key_event))
    return;

  NativeWebKeyboardEventWithLatencyInfo key_event_with_latency(key_event);
  key_event_with_latency.event.isBrowserShortcut = is_shortcut;
  latency_tracker_.OnInputEvent(key_event, &key_event_with_latency.latency);
  input_router_->SendKeyboardEvent(key_event_with_latency);
}
