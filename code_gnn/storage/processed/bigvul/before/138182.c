int AXObject::lineForPosition(const VisiblePosition& position) const {
  if (position.isNull() || !getNode())
    return -1;

  Node* containerNode = position.deepEquivalent().computeContainerNode();
  if (!containerNode->isShadowIncludingInclusiveAncestorOf(getNode()) &&
      !getNode()->isShadowIncludingInclusiveAncestorOf(containerNode))
    return -1;

  int lineCount = -1;
  VisiblePosition currentPosition = position;
  VisiblePosition previousPosition;

  do {
    previousPosition = currentPosition;
    currentPosition =
        previousLinePosition(currentPosition, LayoutUnit(), HasEditableAXRole);
    ++lineCount;
  } while (currentPosition.isNotNull() &&
           !inSameLine(currentPosition, previousPosition));

  return lineCount;
}
