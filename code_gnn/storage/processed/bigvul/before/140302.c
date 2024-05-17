bool Editor::canEditRichly() const {
  return frame()
      .selection()
      .computeVisibleSelectionInDOMTreeDeprecated()
      .isContentRichlyEditable();
}
