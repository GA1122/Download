bool AXNodeObject::isInputImage() const {
  Node* node = this->getNode();
  if (roleValue() == ButtonRole && isHTMLInputElement(node))
    return toHTMLInputElement(*node).type() == InputTypeNames::image;

  return false;
}
