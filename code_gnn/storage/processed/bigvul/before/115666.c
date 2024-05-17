void ClientSession::RecordKeyEvent(const KeyEvent& event) {
  if (event.pressed()) {
    pressed_keys_.insert(event.keycode());
  } else {
    pressed_keys_.erase(event.keycode());
  }
}
