void EventConverterEvdevImpl::ConvertKeyEvent(const input_event& input) {
  if (input.value == kKeyRepeatValue)
    return;

  if (input.code >= BTN_MOUSE && input.code < BTN_JOYSTICK) {
    DispatchMouseButton(input);
    return;
  }

  OnKeyChange(input.code, input.value != kKeyReleaseValue,
              TimeDeltaFromInputEvent(input));
}
