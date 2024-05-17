static bool isCaretAtStartOfWrappedLine(const FrameSelection& selection) {
  if (!selection.computeVisibleSelectionInDOMTreeDeprecated().isCaret())
    return false;
  if (selection.selectionInDOMTree().affinity() != TextAffinity::Downstream)
    return false;
  const Position& position =
      selection.computeVisibleSelectionInDOMTreeDeprecated().start();
  return !inSameLine(PositionWithAffinity(position, TextAffinity::Upstream),
                     PositionWithAffinity(position, TextAffinity::Downstream));
}
