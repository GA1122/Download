bool Editor::canDelete() const {
  FrameSelection& selection = frame().selection();
  return selection.computeVisibleSelectionInDOMTreeDeprecated().isRange() &&
         selection.computeVisibleSelectionInDOMTree().rootEditableElement();
}
