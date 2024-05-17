void RenderWidgetHostViewAura::OnKeyEvent(ui::KeyEvent* event) {
  TRACE_EVENT0("input", "RenderWidgetHostViewAura::OnKeyEvent");
  if (touch_editing_client_ && touch_editing_client_->HandleInputEvent(event))
    return;

  if (popup_child_host_view_ && popup_child_host_view_->NeedsInputGrab()) {
    popup_child_host_view_->OnKeyEvent(event);
    if (event->handled())
      return;
  }

  if (is_fullscreen_ && event->key_code() == ui::VKEY_ESCAPE) {
    if (host_tracker_.get() && !host_tracker_->windows().empty()) {
      aura::Window* host = *(host_tracker_->windows().begin());
      aura::client::FocusClient* client = aura::client::GetFocusClient(host);
      if (client) {
        aura::WindowTracker tracker;
        aura::Window* window = window_;
        tracker.Add(window);
        host->Focus();
        if (!tracker.Contains(window)) {
          event->SetHandled();
          return;
        }
      }
    }
    if (!in_shutdown_) {
      in_shutdown_ = true;
      host_->Shutdown();
    }
  } else {
    if (event->key_code() == ui::VKEY_RETURN) {
      if (event->type() == ui::ET_KEY_RELEASED && !accept_return_character_)
        return;
      accept_return_character_ = event->type() == ui::ET_KEY_PRESSED;
    }

    if (!event->HasNativeEvent()) {
      NativeWebKeyboardEvent webkit_event(
          event->type(),
          event->is_char(),
          event->is_char() ? event->GetCharacter() : event->key_code(),
          event->flags(),
          ui::EventTimeForNow().InSecondsF());
      host_->ForwardKeyboardEvent(webkit_event);
    } else {
      NativeWebKeyboardEvent webkit_event(event);
      host_->ForwardKeyboardEvent(webkit_event);
    }
  }
  event->SetHandled();
}
