bool HTMLInputElement::PatternMismatch() const {
  return willValidate() && input_type_->PatternMismatch(value());
}
