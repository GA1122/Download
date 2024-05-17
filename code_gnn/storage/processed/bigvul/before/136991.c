bool HTMLInputElement::TooShort(const String& value,
                                NeedsToCheckDirtyFlag check) const {
  return input_type_->TooShort(value, check);
}
