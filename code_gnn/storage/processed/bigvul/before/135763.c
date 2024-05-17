bool InputMethodController::SetSelectionOffsets(
    const PlainTextRange& selection_offsets) {
  return SetSelectionOffsets(selection_offsets, TypingContinuation::kEnd);
}
