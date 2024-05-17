bool HTMLInputElement::TooLong() const {
  return willValidate() && TooLong(value(), kCheckDirtyFlag);
}
