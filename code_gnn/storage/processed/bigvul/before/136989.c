bool HTMLInputElement::TooLong(const String& value,
                               NeedsToCheckDirtyFlag check) const {
  return input_type_->TooLong(value, check);
}
