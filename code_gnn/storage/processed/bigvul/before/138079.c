bool AXNodeObject::isNativeTextControl() const {
  Node* node = this->getNode();
  if (!node)
    return false;

  if (isHTMLTextAreaElement(*node))
    return true;

  if (isHTMLInputElement(*node))
    return toHTMLInputElement(node)->isTextField();

  return false;
}
