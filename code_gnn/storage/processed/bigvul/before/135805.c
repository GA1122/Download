void SelectionController::SetCaretAtHitTestResult(
    const HitTestResult& hit_test_result) {
  Node* inner_node = hit_test_result.InnerNode();
  const VisiblePositionInFlatTree& visible_hit_pos =
      VisiblePositionOfHitTestResult(hit_test_result);
  const VisiblePositionInFlatTree& visible_pos =
      visible_hit_pos.IsNull()
          ? CreateVisiblePosition(
                PositionInFlatTree::FirstPositionInOrBeforeNode(inner_node))
          : visible_hit_pos;

  if (visible_pos.IsNull()) {
    UpdateSelectionForMouseDownDispatchingSelectStart(
        inner_node, SelectionInFlatTree(), TextGranularity::kCharacter,
        HandleVisibility::kVisible);
    return;
  }
  UpdateSelectionForMouseDownDispatchingSelectStart(
      inner_node,
      ExpandSelectionToRespectUserSelectAll(
          inner_node, SelectionInFlatTree::Builder()
                          .Collapse(visible_pos.ToPositionWithAffinity())
                          .Build()),
      TextGranularity::kCharacter, HandleVisibility::kVisible);
}
