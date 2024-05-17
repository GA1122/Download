bool HTMLInputElement::RangeUnderflow() const {
  return willValidate() && input_type_->RangeUnderflow(value());
}
