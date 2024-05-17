bool HTMLFormControlElement::matchesValidityPseudoClasses() const {
  return willValidate();
}
