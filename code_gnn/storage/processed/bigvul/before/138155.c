const AtomicString& AXObject::getAttribute(
    const QualifiedName& attribute) const {
  Node* elementNode = getNode();
  if (!elementNode)
    return nullAtom;

  if (!elementNode->isElementNode())
    return nullAtom;

  Element* element = toElement(elementNode);
  return element->fastGetAttribute(attribute);
}
