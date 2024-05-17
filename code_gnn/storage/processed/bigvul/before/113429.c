void InputMethodIBus::DispatchKeyEvent(const base::NativeEvent& native_event) {
  DCHECK(native_event && (native_event->type == KeyPress ||
                          native_event->type == KeyRelease));
  DCHECK(system_toplevel_window_focused());

  guint32 ibus_keyval = 0;
  guint32 ibus_keycode = 0;
  guint32 ibus_state = 0;
  IBusKeyEventFromNativeKeyEvent(
      native_event, &ibus_keyval, &ibus_keycode, &ibus_state);

  if (!context_focused_ ||
      GetTextInputType() == TEXT_INPUT_TYPE_PASSWORD ||
      ibus_client_->GetInputMethodType() ==
      internal::IBusClient::INPUT_METHOD_XKB_LAYOUT) {
    if (native_event->type == KeyPress)
      ProcessUnfilteredKeyPressEvent(native_event, ibus_keyval);
    else
      DispatchKeyEventPostIME(native_event);
    return;
  }

  PendingKeyEventImpl* pending_key =
      new PendingKeyEventImpl(this, native_event, ibus_keyval);
  pending_key_events_.insert(pending_key);

  ibus_client_->SendKeyEvent(context_,
                             ibus_keyval,
                             ibus_keycode,
                             ibus_state,
                             pending_key);

  suppress_next_result_ = false;
}
