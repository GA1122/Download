bool InputMethodIBus::DispatchKeyEvent(const base::NativeEvent& native_event) {
  DCHECK(native_event && (native_event->type == KeyPress ||
                          native_event->type == KeyRelease));
  DCHECK(system_toplevel_window_focused());

  uint32 ibus_keyval = 0;
  uint32 ibus_keycode = 0;
  uint32 ibus_state = 0;
  IBusKeyEventFromNativeKeyEvent(
      native_event,
      &ibus_keyval, &ibus_keycode, &ibus_state);

  if (!context_focused_ || !GetEngine() ||
      GetTextInputType() == TEXT_INPUT_TYPE_PASSWORD ) {
    if (native_event->type == KeyPress) {
      if (ExecuteCharacterComposer(ibus_keyval, ibus_keycode, ibus_state)) {
        ProcessKeyEventPostIME(native_event, ibus_state, true);
        return true;
      }
      ProcessUnfilteredKeyPressEvent(native_event, ibus_state);
    } else {
      DispatchKeyEventPostIME(native_event);
    }
    return true;
  }

  pending_key_events_.insert(current_keyevent_id_);

  XEvent* event = new XEvent;
  *event = *native_event;
  GetEngine()->ProcessKeyEvent(
      ibus_keyval,
      ibus_keycode,
      ibus_state,
      base::Bind(&InputMethodIBus::ProcessKeyEventDone,
                 weak_ptr_factory_.GetWeakPtr(),
                 current_keyevent_id_,
                 base::Owned(event),   
                 ibus_keyval,
                 ibus_keycode,
                 ibus_state));

  ++current_keyevent_id_;

  suppress_next_result_ = false;
  return true;
}
