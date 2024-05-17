bool HTMLInputElement::IsOutOfRange() const {
  return willValidate() && input_type_->IsOutOfRange(value());
}
