bool InputMethodIBus::DispatchFabricatedKeyEvent(const ui::KeyEvent& event) {
  if (event.type() == ET_KEY_PRESSED) {
    ProcessUnfilteredFabricatedKeyPressEvent(
        ET_KEY_PRESSED, event.key_code(), event.flags());
  } else {
    DispatchFabricatedKeyEventPostIME(
        ET_KEY_RELEASED,
        event.key_code(),
        event.flags());
  }
  return true;
}
