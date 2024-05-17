const AtomicString& HTMLLinkElement::GetType() const {
  return getAttribute(typeAttr);
}
