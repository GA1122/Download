void HTMLInputElement::stepDown(int n, ExceptionState& exception_state) {
  input_type_->StepUp(-1.0 * n, exception_state);
}
