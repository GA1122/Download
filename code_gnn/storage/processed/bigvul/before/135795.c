void SelectionController::NotifySelectionChanged() {
  DocumentLifecycle::DisallowTransitionScope disallow_transition(
      frame_->GetDocument()->Lifecycle());

  const SelectionInDOMTree& selection =
      this->Selection().GetSelectionInDOMTree();
  switch (selection.Type()) {
    case kNoSelection:
      selection_state_ = SelectionState::kHaveNotStartedSelection;
      return;
    case kCaretSelection:
      selection_state_ = SelectionState::kPlacedCaret;
      return;
    case kRangeSelection:
      selection_state_ = SelectionState::kExtendedSelection;
      return;
  }
  NOTREACHED() << "We should handle all SelectionType" << selection;
}
