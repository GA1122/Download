bool HTMLMediaElement::IsInteractiveContent() const {
  return FastHasAttribute(kControlsAttr);
}
