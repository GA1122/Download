bool InputMethodWin::DispatchFabricatedKeyEvent(const ui::KeyEvent& event) {
  if (event.is_char()) {
    if (GetTextInputClient()) {
      GetTextInputClient()->InsertChar(event.key_code(),
                                       ui::GetModifiersFromKeyState());
      return true;
    }
  }
  return DispatchFabricatedKeyEventPostIME(event.type(),
                                           event.key_code(),
                                           event.flags());
}
