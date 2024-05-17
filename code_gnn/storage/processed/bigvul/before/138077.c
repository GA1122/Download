bool AXNodeObject::isNativeImage() const {
  Node* node = this->getNode();
  if (!node)
    return false;

  if (isHTMLImageElement(*node))
    return true;

  if (isHTMLPlugInElement(*node))
    return true;

  if (isHTMLInputElement(*node))
    return toHTMLInputElement(*node).type() == InputTypeNames::image;

  return false;
}
