bool AXNodeObject::isNativeSlider() const {
  Node* node = this->getNode();
  if (!node)
    return false;

  if (!isHTMLInputElement(node))
    return false;

  return toHTMLInputElement(node)->type() == InputTypeNames::range;
}
