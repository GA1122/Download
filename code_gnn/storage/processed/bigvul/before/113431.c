void InputMethodIBus::FinishPendingKeyEvent(PendingKeyEventImpl* pending_key) {
  DCHECK(pending_key_events_.count(pending_key));

  pending_key_events_.erase(pending_key);
}
