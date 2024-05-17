void ClientSession::RecordMouseButtonState(const MouseEvent& event) {
  if (event.has_button() && event.has_button_down()) {
    if (event.button() >= 1 && event.button() < MouseEvent::BUTTON_MAX) {
      uint32 button_change = 1 << (event.button() - 1);
      if (event.button_down()) {
        remote_mouse_button_state_ |= button_change;
      } else {
        remote_mouse_button_state_ &= ~button_change;
      }
    }
  }
}
