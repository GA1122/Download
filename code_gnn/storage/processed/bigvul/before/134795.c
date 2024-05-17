void TabletEventConverterEvdev::ConvertKeyEvent(const input_event& input) {
  if (input.code >= BTN_TOOL_PEN && input.code <= BTN_TOOL_LENS) {
    if (input.value == 1)
      stylus_ = input.code;
    else if (input.value == 0)
      stylus_ = 0;
    else
      LOG(WARNING) << "Unexpected value: " << input.value
                   << " for code: " << input.code;
  }

  if (input.code >= BTN_TOUCH && input.code <= BTN_STYLUS2) {
    DispatchMouseButton(input);
    return;
  }
}
