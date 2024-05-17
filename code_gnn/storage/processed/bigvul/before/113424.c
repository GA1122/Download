void InputMethodIBus::AbandonAllPendingKeyEvents() {
  std::set<PendingKeyEventImpl*>::iterator i;
  for (i = pending_key_events_.begin(); i != pending_key_events_.end(); ++i) {
    (*i)->Abandon();
  }
  pending_key_events_.clear();
}
