void InputEventAckWaiter::Reset() {
  event_received_ = false;
  quit_ = base::Closure();
}
