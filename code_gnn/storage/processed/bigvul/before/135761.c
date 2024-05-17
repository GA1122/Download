bool InputMethodController::SetEditableSelectionOffsets(
    const PlainTextRange& selection_offsets) {
  return SetEditableSelectionOffsets(selection_offsets,
                                     TypingContinuation::kEnd);
}
