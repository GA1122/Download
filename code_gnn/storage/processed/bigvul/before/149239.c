bool HTMLFormControlElement::MatchesValidityPseudoClasses() const {
  return willValidate();
}
