bool AXObject::hasAttribute(const QualifiedName& attribute) const {
  Node* elementNode = getNode();
  if (!elementNode)
    return false;

  if (!elementNode->isElementNode())
    return false;

  Element* element = toElement(elementNode);
  return element->fastHasAttribute(attribute);
}
