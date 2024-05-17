void HTMLInputElement::setValueAsDate(double value,
                                      bool is_null,
                                      ExceptionState& exception_state) {
  input_type_->SetValueAsDate(value, exception_state);
}
