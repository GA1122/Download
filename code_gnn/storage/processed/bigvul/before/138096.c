bool AXNodeObject::nameFromContents() const {
  Node* node = getNode();
  if (!node || !node->isElementNode())
    return AXObject::nameFromContents();
  if (isHTMLSelectElement(node))
    return false;
  return AXObject::nameFromContents();
}
