bool Editor::canDeleteRange(const EphemeralRange& range) const {
  if (range.isCollapsed())
    return false;

  Node* startContainer = range.startPosition().computeContainerNode();
  Node* endContainer = range.endPosition().computeContainerNode();
  if (!startContainer || !endContainer)
    return false;

  return hasEditableStyle(*startContainer) && hasEditableStyle(*endContainer);
}
