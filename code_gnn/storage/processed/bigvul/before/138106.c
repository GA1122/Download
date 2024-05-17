AXObject* AXNodeObject::rawNextSibling() const {
  if (!getNode())
    return 0;

  Node* nextSibling = getNode()->nextSibling();
  if (!nextSibling)
    return 0;

  return axObjectCache().getOrCreate(nextSibling);
}
