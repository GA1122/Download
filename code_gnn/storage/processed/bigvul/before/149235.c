bool HTMLFormControlElement::IsRequired() const {
  return FastHasAttribute(kRequiredAttr);
}
