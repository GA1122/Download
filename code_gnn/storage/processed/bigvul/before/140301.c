bool Editor::canEdit() const {
  return frame()
      .selection()
      .computeVisibleSelectionInDOMTreeDeprecated()
      .rootEditableElement();
}
