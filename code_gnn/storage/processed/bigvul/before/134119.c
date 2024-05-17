void InputMethodIBus::ProcessUnfilteredFabricatedKeyPressEvent(
    EventType type,
    KeyboardCode key_code,
    int event_flags) {
  TextInputClient* client = GetTextInputClient();
  DispatchFabricatedKeyEventPostIME(type, key_code, event_flags);

  if (client != GetTextInputClient())
    return;

  client = GetTextInputClient();
  const uint16 ch = ui::GetCharacterFromKeyCode(key_code, event_flags);
  if (client && ch)
    client->InsertChar(ch, event_flags);
}
