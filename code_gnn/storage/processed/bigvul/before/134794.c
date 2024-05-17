void TabletEventConverterEvdev::ConvertAbsEvent(const input_event& input) {
  if (!cursor_)
    return;

  switch (input.code) {
    case ABS_X:
      x_abs_location_ = input.value;
      abs_value_dirty_ = true;
      break;
    case ABS_Y:
      y_abs_location_ = input.value;
      abs_value_dirty_ = true;
      break;
  }
}
