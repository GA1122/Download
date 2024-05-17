void HTMLInputElement::setValue(const String& value,
                                ExceptionState& exception_state,
                                TextFieldEventBehavior event_behavior) {
  if (type() == InputTypeNames::file && !value.IsEmpty()) {
    exception_state.ThrowDOMException(kInvalidStateError,
                                      "This input element accepts a filename, "
                                      "which may only be programmatically set "
                                      "to the empty string.");
    return;
  }
  setValue(value, event_behavior);
}
