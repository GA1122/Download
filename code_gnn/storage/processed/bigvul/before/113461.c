void InputMethodIBus::ProcessKeyEventPostIME(
    const base::NativeEvent& native_event,
    guint32 ibus_keyval,
    bool handled) {
  TextInputClient* client = GetTextInputClient();

  if (!client) {
    DispatchKeyEventPostIME(native_event);
    return;
  }

  if (native_event->type == KeyPress && handled)
    ProcessFilteredKeyPressEvent(native_event);

  if (client != GetTextInputClient())
    return;

  if (HasInputMethodResult())
    ProcessInputMethodResult(native_event, handled);

  if (client != GetTextInputClient())
    return;

  if (native_event->type == KeyPress && !handled)
    ProcessUnfilteredKeyPressEvent(native_event, ibus_keyval);
  else if (native_event->type == KeyRelease)
    DispatchKeyEventPostIME(native_event);
}
