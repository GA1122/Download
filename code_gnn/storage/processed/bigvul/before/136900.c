bool HTMLInputElement::HasBadInput() const {
  return willValidate() && input_type_view_->HasBadInput();
}
