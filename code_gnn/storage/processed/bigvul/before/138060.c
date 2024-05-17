bool AXNodeObject::isDescendantOfElementType(
    HashSet<QualifiedName>& tagNames) const {
  if (!getNode())
    return false;

  for (Element* parent = getNode()->parentElement(); parent;
       parent = parent->parentElement()) {
    if (tagNames.contains(parent->tagQName()))
      return true;
  }
  return false;
}
