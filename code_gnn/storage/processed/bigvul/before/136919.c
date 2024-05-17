bool HTMLInputElement::IsRequiredFormControl() const {
  return input_type_->SupportsRequired() && IsRequired();
}
