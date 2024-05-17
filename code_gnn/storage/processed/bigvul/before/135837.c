bool SelectionEditor::ShouldAlwaysUseDirectionalSelection() const {
  return GetFrame()
      ->GetEditor()
      .Behavior()
      .ShouldConsiderSelectionAsDirectional();
}
