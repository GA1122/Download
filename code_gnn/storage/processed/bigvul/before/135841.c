static Position UpdatePostionAfterAdoptingTextNodeSplit(
    const Position& position,
    const Text& old_node) {
  if (!position.AnchorNode() || position.AnchorNode() != &old_node ||
      !position.IsOffsetInAnchor())
    return position;
  DCHECK_GE(position.OffsetInContainerNode(), 0);
  unsigned position_offset =
      static_cast<unsigned>(position.OffsetInContainerNode());
  unsigned old_length = old_node.length();
  if (position_offset <= old_length)
    return position;
  return Position(ToText(old_node.nextSibling()), position_offset - old_length);
}
