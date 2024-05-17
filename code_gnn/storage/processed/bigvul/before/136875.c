const AtomicString& HTMLInputElement::DefaultValue() const {
  return FastGetAttribute(valueAttr);
}
