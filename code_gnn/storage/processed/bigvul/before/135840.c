static Position UpdatePositionAfterAdoptingTextReplacement(
    const Position& position,
    CharacterData* node,
    unsigned offset,
    unsigned old_length,
    unsigned new_length) {
  if (position.AnchorNode() != node)
    return position;

  if (position.IsBeforeAnchor()) {
    return UpdatePositionAfterAdoptingTextReplacement(
        Position(node, 0), node, offset, old_length, new_length);
  }
  if (position.IsAfterAnchor()) {
    return UpdatePositionAfterAdoptingTextReplacement(
        Position(node, old_length), node, offset, old_length, new_length);
  }

  DCHECK_GE(position.OffsetInContainerNode(), 0);
  unsigned position_offset =
      static_cast<unsigned>(position.OffsetInContainerNode());
  if (position_offset >= offset && position_offset <= offset + old_length)
    position_offset = offset;

  if (position_offset > offset + old_length)
    position_offset = position_offset - old_length + new_length;

  if (position_offset > node->length())
    position_offset = node->length();

  return Position(node, position_offset);
}
