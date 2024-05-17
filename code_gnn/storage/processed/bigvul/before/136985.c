bool HTMLInputElement::SupportsAutofocus() const {
  return input_type_->IsInteractiveContent();
}
