VisiblePosition AXLayoutObject::visiblePositionForIndex(int index) const {
  if (!m_layoutObject)
    return VisiblePosition();

  if (m_layoutObject->isTextControl())
    return toLayoutTextControl(m_layoutObject)
        ->textControlElement()
        ->visiblePositionForIndex(index);

  Node* node = m_layoutObject->node();
  if (!node)
    return VisiblePosition();

  if (index <= 0)
    return createVisiblePosition(firstPositionInOrBeforeNode(node));

  Position start, end;
  bool selected = Range::selectNodeContents(node, start, end);
  if (!selected)
    return VisiblePosition();

  CharacterIterator it(start, end);
  it.advance(index - 1);
  return createVisiblePosition(Position(it.currentContainer(), it.endOffset()),
                               TextAffinity::Upstream);
}
