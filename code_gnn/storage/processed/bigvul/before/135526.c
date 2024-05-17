bool Editor::CanEditRichly() const {
  return GetFrame()
      .Selection()
      .ComputeVisibleSelectionInDOMTreeDeprecated()
      .IsContentRichlyEditable();
}
