bool ClientSession::ShouldIgnoreRemoteKeyboardInput(
    const KeyEvent& event) const {
  if (awaiting_continue_approval_) {
    return event.pressed() ||
        (pressed_keys_.find(event.keycode()) == pressed_keys_.end());
  }
  return false;
}
