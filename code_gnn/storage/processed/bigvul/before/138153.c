const AtomicString& AXObject::getAOMPropertyOrARIAAttribute(
    AOMStringProperty property) const {
  Node* node = this->getNode();
  if (!node || !node->isElementNode())
    return nullAtom;

  return AccessibleNode::getProperty(toElement(node), property);
}
