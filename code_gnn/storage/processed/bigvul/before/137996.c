static VisiblePosition toVisiblePosition(AXObject* obj, int offset) {
  if (!obj->getNode())
    return VisiblePosition();

  Node* node = obj->getNode();
  if (!node->isTextNode()) {
    int childCount = obj->children().size();

    if (childCount == 0) {
      if (!obj->parentObject())
        return VisiblePosition();
      return toVisiblePosition(obj->parentObject(), obj->indexInParent());
    }

    if (offset < 0 || offset > childCount)
      return VisiblePosition();

    int clampedOffset =
        static_cast<unsigned>(offset) > (obj->children().size() - 1)
            ? offset - 1
            : offset;
    AXObject* childObj = obj->children()[clampedOffset];
    Node* childNode = childObj->getNode();
    if (!childNode || !childNode->parentNode())
      return VisiblePosition();

    int adjustedOffset = childNode->nodeIndex();

    if (clampedOffset != offset)
      adjustedOffset++;

    return createVisiblePosition(
        Position::editingPositionOf(childNode->parentNode(), adjustedOffset));
  }

  ContainerNode* parent = node->parentNode();
  if (!parent)
    return VisiblePosition();

  VisiblePosition nodePosition = blink::visiblePositionBeforeNode(*node);
  int nodeIndex = blink::indexForVisiblePosition(nodePosition, parent);
  return blink::visiblePositionForIndex(nodeIndex + offset, parent);
}
