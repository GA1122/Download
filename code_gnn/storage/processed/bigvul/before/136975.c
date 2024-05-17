bool HTMLInputElement::ShouldAutocomplete() const {
  if (autocomplete_ != kUninitialized)
    return autocomplete_ == kOn;
  return TextControlElement::ShouldAutocomplete();
}
