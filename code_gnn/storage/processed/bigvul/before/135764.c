bool InputMethodController::SetSelectionOffsets(
    const PlainTextRange& selection_offsets,
    TypingContinuation typing_continuation) {
  const EphemeralRange range = EphemeralRangeForOffsets(selection_offsets);
  if (range.IsNull())
    return false;

  GetFrame().Selection().SetSelection(
      SelectionInDOMTree::Builder().SetBaseAndExtent(range).Build(),
      SetSelectionData::Builder()
          .SetShouldCloseTyping(typing_continuation == TypingContinuation::kEnd)
          .Build());
  return true;
}
