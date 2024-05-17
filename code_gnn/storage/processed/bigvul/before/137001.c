bool HTMLInputElement::ValueMissing() const {
  return willValidate() && input_type_->ValueMissing(value());
}
