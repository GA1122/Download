void InputMethodIBus::ShowPreeditText() {
  if (suppress_next_result_ || IsTextInputTypeNone())
    return;

  composing_text_ = true;
}
