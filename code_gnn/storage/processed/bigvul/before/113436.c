bool InputMethodIBus::HasInputMethodResult() const {
  return result_text_.length() || composition_changed_;
}
