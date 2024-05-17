void AXLayoutObject::addHiddenChildren() {
  Node* node = this->getNode();
  if (!node)
    return;

  bool shouldInsertHiddenNodes = false;
  for (Node& child : NodeTraversal::childrenOf(*node)) {
    if (!child.layoutObject() && isNodeAriaVisible(&child)) {
      shouldInsertHiddenNodes = true;
      break;
    }
  }

  if (!shouldInsertHiddenNodes)
    return;

  unsigned insertionIndex = 0;
  for (Node& child : NodeTraversal::childrenOf(*node)) {
    if (child.layoutObject()) {
      if (AXObject* childObject = axObjectCache().get(child.layoutObject())) {
        if (childObject->accessibilityIsIgnored()) {
          const auto& children = childObject->children();
          childObject = children.size() ? children.back().get() : 0;
        }
        if (childObject)
          insertionIndex = m_children.find(childObject) + 1;
        continue;
      }
    }

    if (!isNodeAriaVisible(&child))
      continue;

    unsigned previousSize = m_children.size();
    if (insertionIndex > previousSize)
      insertionIndex = previousSize;

    insertChild(axObjectCache().getOrCreate(&child), insertionIndex);
    insertionIndex += (m_children.size() - previousSize);
  }
}
