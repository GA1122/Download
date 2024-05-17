bool AXNodeObject::isControllingVideoElement() const {
  Node* node = this->getNode();
  if (!node)
    return true;

  return isHTMLVideoElement(toParentMediaElement(node));
}
