bool InputMethodLinuxX11::DispatchKeyEvent(
    const base::NativeEvent& native_key_event) {
  EventType event_type = EventTypeFromNative(native_key_event);
  DCHECK(event_type == ET_KEY_PRESSED || event_type == ET_KEY_RELEASED);
  DCHECK(system_toplevel_window_focused());

  if (!GetTextInputClient())
    return DispatchKeyEventPostIME(native_key_event);

  if (input_method_context_->DispatchKeyEvent(native_key_event)) {
    if (event_type == ET_KEY_PRESSED)
      DispatchFabricatedKeyEventPostIME(ET_KEY_PRESSED, VKEY_PROCESSKEY,
                                        EventFlagsFromNative(native_key_event));
    return true;
  }

  const bool handled = DispatchKeyEventPostIME(native_key_event);
  if (event_type == ET_KEY_PRESSED && GetTextInputClient()) {
    uint16 ch = 0;
    const int flags = EventFlagsFromNative(native_key_event);
    if (!(flags & EF_CONTROL_DOWN))
      ch = GetCharacterFromXEvent(native_key_event);
    if (!ch)
      ch = GetCharacterFromKeyCode(KeyboardCodeFromNative(native_key_event),
                                   flags);
    if (ch) {
      GetTextInputClient()->InsertChar(ch, flags);
      return true;
    }
  }
  return handled;
}
