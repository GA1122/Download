void InputMethodController::DeleteSurroundingText(int before, int after) {
  if (!GetEditor().CanEdit())
    return;
  const PlainTextRange selection_offsets(GetSelectionOffsets());
  if (selection_offsets.IsNull())
    return;
  Element* const root_editable_element =
      GetFrame()
          .Selection()
          .ComputeVisibleSelectionInDOMTreeDeprecated()
          .RootEditableElement();
  if (!root_editable_element)
    return;
  int selection_start = static_cast<int>(selection_offsets.Start());
  int selection_end = static_cast<int>(selection_offsets.End());

  if (before > 0 && selection_start > 0) {
    const int start = std::max(selection_start - before, 0);

    const EphemeralRange& range =
        PlainTextRange(0, start).CreateRange(*root_editable_element);
    if (range.IsNull())
      return;
    const Position& position = range.EndPosition();

    const size_t diff = ComputeDistanceToLeftGraphemeBoundary(position);
    const int adjusted_start = start - static_cast<int>(diff);
    if (!SetSelectionOffsets(PlainTextRange(adjusted_start, selection_start)))
      return;
    TypingCommand::DeleteSelection(GetDocument());

    selection_end = selection_end - (selection_start - adjusted_start);
    selection_start = adjusted_start;
  }

  if (after > 0) {
    const PlainTextRange range(0, selection_end + after);
    if (range.IsNull())
      return;
    const EphemeralRange& valid_range =
        range.CreateRange(*root_editable_element);
    if (valid_range.IsNull())
      return;
    const int end =
        PlainTextRange::Create(*root_editable_element, valid_range).End();
    const Position& position = valid_range.EndPosition();

    const size_t diff = ComputeDistanceToRightGraphemeBoundary(position);
    const int adjusted_end = end + static_cast<int>(diff);
    if (!SetSelectionOffsets(PlainTextRange(selection_end, adjusted_end)))
      return;
    TypingCommand::DeleteSelection(GetDocument());
  }

  SetSelectionOffsets(PlainTextRange(selection_start, selection_end));
}
