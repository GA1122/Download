bool SelectionController::UpdateSelectionForMouseDownDispatchingSelectStart(
    Node* target_node,
    const SelectionInFlatTree& selection,
    TextGranularity granularity,
    HandleVisibility handle_visibility) {
  if (target_node && target_node->GetLayoutObject() &&
      !target_node->GetLayoutObject()->IsSelectable())
    return false;

  const VisibleSelectionInFlatTree& visible_selection =
      CreateVisibleSelection(selection);

  if (DispatchSelectStart(target_node) != DispatchEventResult::kNotCanceled)
    return false;

  if (!this->Selection().IsAvailable())
    return false;

  if (!visible_selection.IsValidFor(this->Selection().GetDocument()))
    return false;

  if (visible_selection.IsRange()) {
    selection_state_ = SelectionState::kExtendedSelection;
    SetNonDirectionalSelectionIfNeeded(
        selection, granularity, kDoNotAdjustEndpoints, handle_visibility);

    return true;
  }

  selection_state_ = SelectionState::kPlacedCaret;
  SetNonDirectionalSelectionIfNeeded(selection, TextGranularity::kCharacter,
                                     kDoNotAdjustEndpoints, handle_visibility);
  return true;
}
