const AtomicString& AXLayoutObject::accessKey() const {
  Node* node = m_layoutObject->node();
  if (!node)
    return nullAtom;
  if (!node->isElementNode())
    return nullAtom;
  return toElement(node)->getAttribute(accesskeyAttr);
}
