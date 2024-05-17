bool HTMLFormElement::NoValidate() const {
  return FastHasAttribute(novalidateAttr);
}
