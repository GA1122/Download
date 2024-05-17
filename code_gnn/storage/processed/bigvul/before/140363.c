EphemeralRange Editor::selectedRange() {
  return frame()
      .selection()
      .computeVisibleSelectionInDOMTreeDeprecated()
      .toNormalizedEphemeralRange();
}
