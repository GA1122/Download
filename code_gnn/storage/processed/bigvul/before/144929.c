void RenderWidgetHostViewAura::InsertChar(const ui::KeyEvent& event) {
  if (popup_child_host_view_ && popup_child_host_view_->NeedsInputGrab()) {
    popup_child_host_view_->InsertChar(event);
    return;
  }

  if (host_ && (event_handler_->accept_return_character() ||
                event.GetCharacter() != ui::VKEY_RETURN)) {
    ForwardKeyboardEvent(NativeWebKeyboardEvent(event, event.GetCharacter()),
                         nullptr);
  }
}
