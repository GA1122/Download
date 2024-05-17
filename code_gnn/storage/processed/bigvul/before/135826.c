void SelectionEditor::DidUpdateCharacterData(CharacterData* node,
                                             unsigned offset,
                                             unsigned old_length,
                                             unsigned new_length) {
  if (selection_.IsNone() || !node || !node->isConnected()) {
    DidFinishDOMMutation();
    return;
  }
  const Position& new_base = UpdatePositionAfterAdoptingTextReplacement(
      selection_.base_, node, offset, old_length, new_length);
  const Position& new_extent = UpdatePositionAfterAdoptingTextReplacement(
      selection_.extent_, node, offset, old_length, new_length);
  DidFinishTextChange(new_base, new_extent);
}
