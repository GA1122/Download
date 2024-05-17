bool HTMLInputElement::SupportLabels() const {
  return input_type_->IsInteractiveContent();
}
