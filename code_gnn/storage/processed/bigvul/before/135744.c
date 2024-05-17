PlainTextRange InputMethodController::GetSelectionOffsets() const {
  EphemeralRange range = FirstEphemeralRangeOf(
      GetFrame().Selection().ComputeVisibleSelectionInDOMTreeDeprecated());
  if (range.IsNull())
    return PlainTextRange();
  ContainerNode* const editable = RootEditableElementOrTreeScopeRootNodeOf(
      GetFrame().Selection().ComputeVisibleSelectionInDOMTree().Base());
  DCHECK(editable);
  return PlainTextRange::Create(*editable, range);
}
