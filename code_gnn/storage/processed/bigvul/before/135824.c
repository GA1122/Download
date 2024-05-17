void SelectionEditor::DidMergeTextNodes(
    const Text& merged_node,
    const NodeWithIndex& node_to_be_removed_with_index,
    unsigned old_length) {
  if (selection_.IsNone()) {
    DidFinishDOMMutation();
    return;
  }
  const Position& new_base = UpdatePostionAfterAdoptingTextNodesMerged(
      selection_.base_, merged_node, node_to_be_removed_with_index, old_length);
  const Position& new_extent = UpdatePostionAfterAdoptingTextNodesMerged(
      selection_.extent_, merged_node, node_to_be_removed_with_index,
      old_length);
  DidFinishTextChange(new_base, new_extent);
}
