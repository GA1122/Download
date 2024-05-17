void RenderWidgetHostViewAura::InsertChar(base::char16 ch, int flags) {
  if (popup_child_host_view_ && popup_child_host_view_->NeedsInputGrab()) {
    popup_child_host_view_->InsertChar(ch, flags);
    return;
  }

  if (host_ && (accept_return_character_ || ch != ui::VKEY_RETURN)) {
    double now = ui::EventTimeForNow().InSecondsF();
    NativeWebKeyboardEvent webkit_event(ui::ET_KEY_PRESSED,
                                        true  ,
                                        ch,
                                        flags,
                                        now);
    host_->ForwardKeyboardEvent(webkit_event);
  }
}
