static bool IsCaretAtStartOfWrappedLine(const FrameSelection& selection) {
  if (!selection.ComputeVisibleSelectionInDOMTree().IsCaret())
    return false;
  if (selection.GetSelectionInDOMTree().Affinity() != TextAffinity::kDownstream)
    return false;
  const Position& position =
      selection.ComputeVisibleSelectionInDOMTree().Start();
  return !InSameLine(PositionWithAffinity(position, TextAffinity::kUpstream),
                     PositionWithAffinity(position, TextAffinity::kDownstream));
}
