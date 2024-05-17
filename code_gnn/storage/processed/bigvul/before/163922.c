InlineBoxPosition AdjustInlineBoxPositionForPrimaryDirection(
    InlineBox* inline_box,
    int caret_offset) {
  if (caret_offset == inline_box->CaretRightmostOffset()) {
    InlineBox* const next_box = inline_box->NextLeafChild();
    if (!next_box || next_box->BidiLevel() >= inline_box->BidiLevel())
      return InlineBoxPosition(inline_box, caret_offset);

    const unsigned level = next_box->BidiLevel();
    InlineBox* const prev_box =
        InlineBoxTraversal::FindLeftBidiRun(*inline_box, level);

    if (prev_box && prev_box->BidiLevel() == level)
      return InlineBoxPosition(inline_box, caret_offset);

    InlineBox* const result_box =
        InlineBoxTraversal::FindRightBoundaryOfEntireBidiRun(*inline_box,
                                                             level);
    return InlineBoxPosition(result_box, result_box->CaretRightmostOffset());
  }

  if (IsStartOfDifferentDirection(inline_box))
    return InlineBoxPosition(inline_box, caret_offset);

  const unsigned level = inline_box->PrevLeafChild()->BidiLevel();
  InlineBox* const next_box =
      InlineBoxTraversal::FindRightBidiRun(*inline_box, level);

  if (next_box && next_box->BidiLevel() == level)
    return InlineBoxPosition(inline_box, caret_offset);

  InlineBox* const result_box =
      InlineBoxTraversal::FindLeftBoundaryOfEntireBidiRun(*inline_box, level);
  return InlineBoxPosition(result_box, result_box->CaretLeftmostOffset());
}
