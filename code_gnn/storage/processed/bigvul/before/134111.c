void InputMethodIBus::IBusKeyEventFromNativeKeyEvent(
    const base::NativeEvent& native_event,
    uint32* ibus_keyval,
    uint32* ibus_keycode,
    uint32* ibus_state) {
  DCHECK(native_event);   
  XKeyEvent* x_key = GetKeyEvent(native_event);

  KeySym keysym = NoSymbol;
  ::XLookupString(x_key, NULL, 0, &keysym, NULL);
  *ibus_keyval = keysym;
  *ibus_keycode = x_key->keycode;
  *ibus_state = IBusStateFromXState(x_key->state);
  if (native_event->type == KeyRelease)
    *ibus_state |= kIBusReleaseMask;
}
