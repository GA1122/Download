bool InputMethodController::SetEditableSelectionOffsets(
    const PlainTextRange& selection_offsets,
    TypingContinuation typing_continuation) {
  if (!GetEditor().CanEdit())
    return false;
  return SetSelectionOffsets(selection_offsets, typing_continuation);
}
