bool OmniboxViewViews::DirectionAwareSelectionAtEnd() const {
  return TextAndUIDirectionMatch() ? SelectionAtEnd() : SelectionAtBeginning();
}
