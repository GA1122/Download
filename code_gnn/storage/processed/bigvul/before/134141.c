bool InputMethodLinuxX11::DispatchFabricatedKeyEvent(
    const ui::KeyEvent& event) {
  if (DispatchFabricatedKeyEventPostIME(event.type(), event.key_code(),
                                        event.flags()))
    return true;

  if (event.type() == ET_KEY_PRESSED && GetTextInputClient()) {
    const uint16 ch = event.GetCharacter();
    if (ch) {
      GetTextInputClient()->InsertChar(ch, event.flags());
      return true;
    }
  }

  return false;
}
