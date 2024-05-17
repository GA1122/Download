static SelectionInDOMTree CorrectedSelectionAfterCommand(
    const VisibleSelection& passed_selection,
    Document* document) {
  if (!passed_selection.Base().IsConnected() ||
      !passed_selection.Extent().IsConnected() ||
      passed_selection.Base().GetDocument() != document ||
      passed_selection.Base().GetDocument() !=
          passed_selection.Extent().GetDocument())
    return SelectionInDOMTree();
  return passed_selection.AsSelection();
}
