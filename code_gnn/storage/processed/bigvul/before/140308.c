void Editor::changeSelectionAfterCommand(
    const SelectionInDOMTree& newSelection,
    FrameSelection::SetSelectionOptions options) {
  if (newSelection.isNone())
    return;

  bool selectionDidNotChangeDOMPosition =
      newSelection == frame().selection().selectionInDOMTree();
  frame().selection().setSelection(newSelection, options);

  if (selectionDidNotChangeDOMPosition) {
    client().respondToChangedSelection(
        m_frame,
        frame()
            .selection()
            .computeVisibleSelectionInDOMTreeDeprecated()
            .getSelectionType());
  }
}
