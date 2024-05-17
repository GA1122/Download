bool HTMLInputElement::RangeOverflow() const {
  return willValidate() && input_type_->RangeOverflow(value());
}
