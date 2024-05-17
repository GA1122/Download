bool HTMLMediaElement::IsInteractiveContent() const {
  return FastHasAttribute(controlsAttr);
}
