bool Editor::CanDelete() const {
  FrameSelection& selection = GetFrame().Selection();
  return selection.ComputeVisibleSelectionInDOMTreeDeprecated().IsRange() &&
         selection.ComputeVisibleSelectionInDOMTree().RootEditableElement();
}
