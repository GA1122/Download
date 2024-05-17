void IBusKeyEventFromNativeKeyEvent(const base::NativeEvent& native_event,
                                    guint32* ibus_keyval,
                                    guint32* ibus_keycode,
                                    guint32* ibus_state) {
  DCHECK(native_event);   
  XKeyEvent* x_key = GetKeyEvent(native_event);

  KeySym keysym = NoSymbol;
  ::XLookupString(x_key, NULL, 0, &keysym, NULL);
  *ibus_keyval = keysym;
  *ibus_keycode = x_key->keycode;
  *ibus_state = IBusStateFromXFlags(x_key->state);
  if (native_event->type == KeyRelease)
    *ibus_state |= ui::kIBusReleaseMask;
}
