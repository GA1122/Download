void EventConverterEvdevImpl::DispatchMouseButton(const input_event& input) {
  if (!cursor_)
    return;

  OnButtonChange(input.code, input.value, TimeDeltaFromInputEvent(input));
}
