bool HTMLMediaElement::isInteractiveContent() const {
  return fastHasAttribute(controlsAttr);
}
