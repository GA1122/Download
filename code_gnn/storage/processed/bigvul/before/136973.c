bool HTMLInputElement::ShouldAppearChecked() const {
  return checked() && input_type_->IsCheckable();
}
