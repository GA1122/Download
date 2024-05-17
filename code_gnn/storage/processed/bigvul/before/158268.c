void RenderWidgetHostImpl::ForwardKeyboardEventWithCommands(
    const NativeWebKeyboardEvent& key_event,
    const ui::LatencyInfo& latency,
    const std::vector<EditCommand>* commands,
    bool* update_event) {
  TRACE_EVENT0("input", "RenderWidgetHostImpl::ForwardKeyboardEvent");
  if (owner_delegate_ &&
      !owner_delegate_->MayRenderWidgetForwardKeyboardEvent(key_event)) {
    return;
  }

  if (ShouldDropInputEvents())
    return;

  if (!process_->IsInitializedAndNotDead())
    return;

  if (KeyPressListenersHandleEvent(key_event)) {
    if (key_event.GetType() == WebKeyboardEvent::kRawKeyDown)
      suppress_events_until_keydown_ = true;
    return;
  }

  if (!WebInputEvent::IsKeyboardEventType(key_event.GetType()))
    return;

  if (suppress_events_until_keydown_) {
    if (key_event.GetType() == WebKeyboardEvent::kKeyUp ||
        key_event.GetType() == WebKeyboardEvent::kChar)
      return;
    DCHECK(key_event.GetType() == WebKeyboardEvent::kRawKeyDown ||
           key_event.GetType() == WebKeyboardEvent::kKeyDown);
    suppress_events_until_keydown_ = false;
  }

  bool is_shortcut = false;

  if (delegate_ && !key_event.skip_in_browser) {
    if (key_event.GetType() == WebKeyboardEvent::kRawKeyDown)
      suppress_events_until_keydown_ = true;

    switch (delegate_->PreHandleKeyboardEvent(key_event)) {
      case KeyboardEventProcessingResult::HANDLED:
        return;
#if defined(USE_AURA)
      case KeyboardEventProcessingResult::HANDLED_DONT_UPDATE_EVENT:
        if (update_event)
          *update_event = false;
        return;
#endif
      case KeyboardEventProcessingResult::NOT_HANDLED:
        break;
      case KeyboardEventProcessingResult::NOT_HANDLED_IS_SHORTCUT:
        is_shortcut = true;
        break;
    }

    if (key_event.GetType() == WebKeyboardEvent::kRawKeyDown)
      suppress_events_until_keydown_ = false;
  }

  auto* touch_emulator = GetExistingTouchEmulator();
  if (touch_emulator && touch_emulator->HandleKeyboardEvent(key_event))
    return;
  NativeWebKeyboardEventWithLatencyInfo key_event_with_latency(key_event,
                                                               latency);
  key_event_with_latency.event.is_browser_shortcut = is_shortcut;
  DispatchInputEventWithLatencyInfo(key_event, &key_event_with_latency.latency);
  if (commands && !commands->empty()) {
    GetWidgetInputHandler()->SetEditCommandsForNextKeyEvent(*commands);
  }
  input_router_->SendKeyboardEvent(key_event_with_latency);
}
