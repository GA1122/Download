bool Editor::CanEdit() const {
  return GetFrame()
      .Selection()
      .ComputeVisibleSelectionInDOMTreeDeprecated()
      .RootEditableElement();
}
