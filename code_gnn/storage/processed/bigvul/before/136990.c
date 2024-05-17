bool HTMLInputElement::TooShort() const {
  return willValidate() && TooShort(value(), kCheckDirtyFlag);
}
