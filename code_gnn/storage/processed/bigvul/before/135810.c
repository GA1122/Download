void SelectionController::UpdateSelectionForMouseDrag(
    const HitTestResult& hit_test_result,
    Node* mouse_press_node,
    const LayoutPoint& drag_start_pos,
    const IntPoint& last_known_mouse_position) {
  if (!mouse_down_may_start_select_)
    return;

  Node* target = hit_test_result.InnerNode();
  if (!target)
    return;

  frame_->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  const PositionWithAffinity& raw_target_position =
      Selection().SelectionHasFocus()
          ? PositionRespectingEditingBoundary(
                Selection().ComputeVisibleSelectionInDOMTree().Start(),
                hit_test_result.LocalPoint(), target)
          : PositionWithAffinity();
  VisiblePositionInFlatTree target_position = CreateVisiblePosition(
      FromPositionInDOMTree<EditingInFlatTreeStrategy>(raw_target_position));
  if (target_position.IsNull())
    return;


  if (ShouldRespectSVGTextBoundaries(*target, Selection()))
    return;

  if (selection_state_ == SelectionState::kHaveNotStartedSelection &&
      DispatchSelectStart(target) != DispatchEventResult::kNotCanceled)
    return;


  const bool should_extend_selection =
      selection_state_ == SelectionState::kExtendedSelection;
  selection_state_ = SelectionState::kExtendedSelection;

  const VisibleSelectionInFlatTree& visible_selection =
      Selection().ComputeVisibleSelectionInFlatTree();
  if (visible_selection.IsNone()) {
    return;
  }

  const PositionInFlatTree& adjusted_position =
      AdjustPositionRespectUserSelectAll(target, visible_selection.Start(),
                                         visible_selection.End(),
                                         target_position.DeepEquivalent());
  const SelectionInFlatTree& adjusted_selection =
      should_extend_selection
          ? ExtendSelectionAsDirectional(adjusted_position, visible_selection,
                                         Selection().Granularity())
          : SelectionInFlatTree::Builder().Collapse(adjusted_position).Build();

  SetNonDirectionalSelectionIfNeeded(
      adjusted_selection, Selection().Granularity(),
      kAdjustEndpointsAtBidiBoundary, HandleVisibility::kNotVisible);
}
