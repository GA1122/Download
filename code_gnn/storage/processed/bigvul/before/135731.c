PlainTextRange InputMethodController::CreateSelectionRangeForSetComposition(
    int selection_start,
    int selection_end,
    size_t text_length) const {
  const int selection_offsets_start =
      static_cast<int>(GetSelectionOffsets().Start());
  const int start = selection_offsets_start + selection_start;
  const int end = selection_offsets_start + selection_end;
  return CreateRangeForSelection(start, end, text_length);
}
