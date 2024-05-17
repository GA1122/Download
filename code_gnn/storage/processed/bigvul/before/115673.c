MATCHER_P(EqualsMouseUpEvent, button, "") {
  return arg.button() == button && !arg.button_down();
}
