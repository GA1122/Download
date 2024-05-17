void InputMethodIBus::ProcessUnfilteredKeyPressEvent(
    const base::NativeEvent& native_event,
    uint32 ibus_state) {
  DCHECK(native_event);

  TextInputClient* client = GetTextInputClient();
  DispatchKeyEventPostIME(native_event);

  if (client != GetTextInputClient())
    return;

  const uint32 event_flags = EventFlagsFromXState(ibus_state);

  client = GetTextInputClient();

  uint16 ch = 0;
  if (!(event_flags & ui::EF_CONTROL_DOWN))
    ch = ui::GetCharacterFromXEvent(native_event);
  if (!ch) {
    ch = ui::GetCharacterFromKeyCode(
        ui::KeyboardCodeFromNative(native_event), event_flags);
  }

  if (client && ch)
    client->InsertChar(ch, event_flags);
}
