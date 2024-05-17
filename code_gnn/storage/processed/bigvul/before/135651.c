bool FrameSelection::NeedsLayoutSelectionUpdate() const {
  return layout_selection_->HasPendingSelection();
}
