IntRect FrameSelection::ComputeRectToScroll(
    RevealExtentOption reveal_extent_option) {
  const VisibleSelection& selection = ComputeVisibleSelectionInDOMTree();
  if (selection.IsCaret())
    return AbsoluteCaretBounds();
  DCHECK(selection.IsRange());
  if (reveal_extent_option == kRevealExtent)
    return AbsoluteCaretBoundsOf(CreateVisiblePosition(selection.Extent()));
  layout_selection_->SetHasPendingSelection();
  return layout_selection_->AbsoluteSelectionBounds();
}
