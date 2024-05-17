void InputMethodIBus::ProcessKeyEventDone(uint32 id,
                                          XEvent* event,
                                          uint32 ibus_keyval,
                                          uint32 ibus_keycode,
                                          uint32 ibus_state,
                                          bool is_handled) {
  DCHECK(event);
  std::set<uint32>::iterator it = pending_key_events_.find(id);

  if (it == pending_key_events_.end())
    return;   

  if (event->type == KeyPress) {
    if (is_handled) {
      character_composer_.Reset();
    } else {
      is_handled = ExecuteCharacterComposer(ibus_keyval, ibus_keycode,
                                            ibus_state);
    }
  }

  if (event->type == KeyPress || event->type == KeyRelease)
    ProcessKeyEventPostIME(event, ibus_state, is_handled);

  pending_key_events_.erase(id);
}
