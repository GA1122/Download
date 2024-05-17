int CalculateBeforeDeletionLengthsInCodePoints(
    const String& text,
    const int before_length_in_code_points,
    const int selection_start) {
  DCHECK_GE(before_length_in_code_points, 0);
  DCHECK_GE(selection_start, 0);
  DCHECK_LE(selection_start, static_cast<int>(text.length()));

  const UChar* u_text = text.Characters16();
  BackwardCodePointStateMachine backward_machine;
  int counter = before_length_in_code_points;
  int deletion_start = selection_start;
  while (counter > 0 && deletion_start > 0) {
    const TextSegmentationMachineState state =
        backward_machine.FeedPrecedingCodeUnit(u_text[deletion_start - 1]);
    if (state == TextSegmentationMachineState::kInvalid)
      return kInvalidDeletionLength;

    if (backward_machine.AtCodePointBoundary())
      --counter;
    --deletion_start;
  }
  if (!backward_machine.AtCodePointBoundary())
    return kInvalidDeletionLength;

  const int offset = backward_machine.GetBoundaryOffset();
  DCHECK_EQ(-offset, selection_start - deletion_start);
  return -offset;
}
