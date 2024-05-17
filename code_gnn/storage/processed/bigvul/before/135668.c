bool FrameSelection::SelectWordAroundPosition(const VisiblePosition& position) {
  static const EWordSide kWordSideList[2] = {kRightWordIfOnBoundary,
                                             kLeftWordIfOnBoundary};
  for (EWordSide word_side : kWordSideList) {
    VisiblePosition start = StartOfWord(position, word_side);
    VisiblePosition end = EndOfWord(position, word_side);
    String text =
        PlainText(EphemeralRange(start.DeepEquivalent(), end.DeepEquivalent()));
    if (!text.IsEmpty() && !IsSeparator(text.CharacterStartingAt(0))) {
      SetSelection(SelectionInDOMTree::Builder()
                       .Collapse(start.ToPositionWithAffinity())
                       .Extend(end.DeepEquivalent())
                       .Build(),
                   SetSelectionData::Builder()
                       .SetShouldCloseTyping(true)
                       .SetShouldClearTypingStyle(true)
                       .SetGranularity(TextGranularity::kWord)
                       .Build());
      return true;
    }
  }

  return false;
}