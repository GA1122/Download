void RenderWidgetHostImpl::ForwardKeyboardEvent(
    const NativeWebKeyboardEvent& key_event) {
  TRACE_EVENT0("renderer_host", "RenderWidgetHostImpl::ForwardKeyboardEvent");
  if (ignore_input_events_ || process_->IgnoreInputEvents())
    return;

  if (key_event.type == WebKeyboardEvent::Char &&
      (key_event.windowsKeyCode == ui::VKEY_RETURN ||
       key_event.windowsKeyCode == ui::VKEY_SPACE)) {
    OnUserGesture();
  }

  if (WebInputEvent::isKeyboardEventType(key_event.type)) {
    if (suppress_next_char_events_) {
      if (key_event.type == WebKeyboardEvent::Char)
        return;
      suppress_next_char_events_ = false;
    }

    bool is_keyboard_shortcut = false;
    if (delegate_ && !key_event.skip_in_browser) {
      if (key_event.type == WebKeyboardEvent::RawKeyDown)
        suppress_next_char_events_ = true;

      if (delegate_->PreHandleKeyboardEvent(key_event, &is_keyboard_shortcut))
        return;

      if (key_event.type == WebKeyboardEvent::RawKeyDown)
        suppress_next_char_events_ = false;
    }

    if (!process_->HasConnection())
      return;

    key_queue_.push_back(key_event);
    HISTOGRAM_COUNTS_100("Renderer.KeyboardQueueSize", key_queue_.size());

    gesture_event_filter_->FlingHasBeenHalted();

    ForwardInputEvent(key_event, sizeof(WebKeyboardEvent),
                      is_keyboard_shortcut);
  }
}
