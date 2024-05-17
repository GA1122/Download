void RenderWidgetHostViewAura::InsertChar(char16 ch, int flags) {
  if (popup_child_host_view_ && popup_child_host_view_->NeedsInputGrab()) {
    popup_child_host_view_->InsertChar(ch, flags);
    return;
  }

  if (host_) {
    NativeWebKeyboardEvent webkit_event(ui::ET_KEY_PRESSED,
                                        true  ,
                                        ch,
                                        flags,
                                        base::Time::Now().ToDoubleT());
    host_->ForwardKeyboardEvent(webkit_event);
  }
}
