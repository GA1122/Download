String HTMLInputElement::SanitizeValue(const String& proposed_value) const {
  return input_type_->SanitizeValue(proposed_value);
}
