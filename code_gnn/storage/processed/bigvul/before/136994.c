bool HTMLInputElement::TypeMismatch() const {
  return willValidate() && input_type_->TypeMismatch();
}
