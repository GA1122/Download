int CalculateAfterDeletionLengthsInCodePoints(
    const String& text,
    const int after_length_in_code_points,
    const int selection_end) {
  DCHECK_GE(after_length_in_code_points, 0);
  DCHECK_GE(selection_end, 0);
  const int length = text.length();
  DCHECK_LE(selection_end, length);

  const UChar* u_text = text.Characters16();
  ForwardCodePointStateMachine forward_machine;
  int counter = after_length_in_code_points;
  int deletion_end = selection_end;
  while (counter > 0 && deletion_end < length) {
    const TextSegmentationMachineState state =
        forward_machine.FeedFollowingCodeUnit(u_text[deletion_end]);
    if (state == TextSegmentationMachineState::kInvalid)
      return kInvalidDeletionLength;

    if (forward_machine.AtCodePointBoundary())
      --counter;
    ++deletion_end;
  }
  if (!forward_machine.AtCodePointBoundary())
    return kInvalidDeletionLength;

  const int offset = forward_machine.GetBoundaryOffset();
  DCHECK_EQ(offset, deletion_end - selection_end);
  return offset;
}
