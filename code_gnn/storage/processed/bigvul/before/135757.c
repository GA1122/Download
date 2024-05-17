Element* RootEditableElementOfSelection(const FrameSelection& frameSelection) {
  const SelectionInDOMTree& selection = frameSelection.GetSelectionInDOMTree();
  if (selection.IsNone())
    return nullptr;
  if (Element* editable = RootEditableElementOf(selection.Base()))
    return editable;


  frameSelection.GetDocument().UpdateStyleAndLayoutIgnorePendingStylesheets();
  const VisibleSelection& visibleSeleciton =
      frameSelection.ComputeVisibleSelectionInDOMTree();
  return RootEditableElementOf(visibleSeleciton.Start());
}
