void InputMethodIBus::OnForwardKeyEvent(IBusInputContext* context,
                                        guint keyval,
                                        guint keycode,
                                        guint state) {
  DCHECK_EQ(context_, context);

  KeyboardCode ui_key_code = KeyboardCodeFromXKeysym(keyval);
  if (!ui_key_code)
    return;

  const EventType event_type =
      (state & kIBusReleaseMask) ? ET_KEY_RELEASED : ET_KEY_PRESSED;
  const int event_flags = EventFlagsFromXFlags(state);

  if (event_type == ET_KEY_PRESSED) {
    ProcessUnfilteredFabricatedKeyPressEvent(
        event_type, ui_key_code, event_flags, keyval);
  } else {
    DispatchFabricatedKeyEventPostIME(event_type, ui_key_code, event_flags);
  }
}
