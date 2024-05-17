bool HTMLInputElement::IsInRange() const {
  return willValidate() && input_type_->IsInRange(value());
}
