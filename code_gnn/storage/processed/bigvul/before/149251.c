bool HTMLFormControlElement::SupportsFocus() const {
  return !IsDisabledFormControl();
}
