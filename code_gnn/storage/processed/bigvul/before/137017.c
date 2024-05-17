void HTMLInputElement::setRangeText(const String& replacement,
                                    ExceptionState& exception_state) {
  if (!input_type_->SupportsSelectionAPI()) {
    exception_state.ThrowDOMException(kInvalidStateError,
                                      "The input element's type ('" +
                                          input_type_->FormControlType() +
                                          "') does not support selection.");
    return;
  }

  TextControlElement::setRangeText(replacement, exception_state);
}
