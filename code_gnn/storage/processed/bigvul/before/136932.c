bool HTMLInputElement::MatchesReadWritePseudoClass() const {
  return input_type_->SupportsReadOnly() && !IsReadOnly();
}
