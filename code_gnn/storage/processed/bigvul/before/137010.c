String HTMLInputElement::selectionDirectionForBinding(
    ExceptionState& exception_state) const {
  if (!input_type_->SupportsSelectionAPI()) {
    return String();
  }
  return TextControlElement::selectionDirection();
}
