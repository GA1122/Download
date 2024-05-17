EphemeralRange Editor::SelectedRange() {
  return GetFrame()
      .Selection()
      .ComputeVisibleSelectionInDOMTreeDeprecated()
      .ToNormalizedEphemeralRange();
}
