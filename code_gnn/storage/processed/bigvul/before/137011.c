unsigned HTMLInputElement::selectionEndForBinding(
    bool& is_null,
    ExceptionState& exception_state) const {
  if (!input_type_->SupportsSelectionAPI()) {
    is_null = true;
    return 0;
  }
  return TextControlElement::selectionEnd();
}
