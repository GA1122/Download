bool HTMLInputElement::ShouldSaveAndRestoreFormControlState() const {
  if (!input_type_->ShouldSaveAndRestoreFormControlState())
    return false;
  return TextControlElement::ShouldSaveAndRestoreFormControlState();
}
