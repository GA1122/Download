bool HTMLInputElement::RecalcWillValidate() const {
  return input_type_->SupportsValidation() &&
         TextControlElement::RecalcWillValidate();
}
