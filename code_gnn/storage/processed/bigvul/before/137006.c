bool HTMLInputElement::checked() const {
  input_type_->ReadingChecked();
  return is_checked_;
}
