bool HTMLFormControlElement::matchesEnabledPseudoClass() const {
  return !isDisabledFormControl();
}
