void TouchEventConverterEvdev::ProcessKey(const input_event& input) {
  switch (input.code) {
    case BTN_TOUCH:
      break;
    default:
      NOTIMPLEMENTED() << "invalid code for EV_KEY: " << input.code;
  }
}
