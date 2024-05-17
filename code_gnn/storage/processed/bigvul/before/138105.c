AXObject* AXNodeObject::rawFirstChild() const {
  if (!getNode())
    return 0;

  Node* firstChild = getNode()->firstChild();

  if (!firstChild)
    return 0;

  return axObjectCache().getOrCreate(firstChild);
}
