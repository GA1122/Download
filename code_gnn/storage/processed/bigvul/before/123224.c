ui::EventResult RenderWidgetHostViewAura::OnKeyEvent(ui::KeyEvent* event) {
  TRACE_EVENT0("browser", "RenderWidgetHostViewAura::OnKeyEvent");
  if (popup_child_host_view_ && popup_child_host_view_->NeedsInputGrab() &&
      popup_child_host_view_->OnKeyEvent(event))
    return ui::ER_HANDLED;

  if (is_fullscreen_ && event->key_code() == ui::VKEY_ESCAPE) {
    if (host_tracker_.get() && !host_tracker_->windows().empty()) {
      aura::Window* host = *(host_tracker_->windows().begin());
      aura::client::FocusClient* client = aura::client::GetFocusClient(host);
      if (client)
        host->Focus();
    }
    if (!in_shutdown_) {
      in_shutdown_ = true;
      host_->Shutdown();
    }
  } else {
    if (!event->HasNativeEvent()) {
      NativeWebKeyboardEvent webkit_event(
          event->type(),
          false  ,
          event->GetCharacter() ? event->GetCharacter() : event->key_code(),
          event->flags(),
          base::Time::Now().ToDoubleT());
      host_->ForwardKeyboardEvent(webkit_event);
    } else {
      NativeWebKeyboardEvent webkit_event(event);
      host_->ForwardKeyboardEvent(webkit_event);
    }
  }
  return ui::ER_HANDLED;
}
