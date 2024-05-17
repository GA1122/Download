void InputMethodController::SelectComposition() const {
  const EphemeralRange range = CompositionEphemeralRange();
  if (range.IsNull())
    return;

  GetFrame().Selection().SetSelection(
      SelectionInDOMTree::Builder().SetBaseAndExtent(range).Build());
}
