bool InputMethodController::MoveCaret(int new_caret_position) {
  GetDocument().UpdateStyleAndLayoutIgnorePendingStylesheets();
  PlainTextRange selected_range =
      CreateRangeForSelection(new_caret_position, new_caret_position, 0);
  if (selected_range.IsNull())
    return false;
  return SetEditableSelectionOffsets(selected_range);
}
