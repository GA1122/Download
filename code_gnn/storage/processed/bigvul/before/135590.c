void Editor::RespondToChangedSelection(const Position& old_selection_start,
                                       TypingContinuation typing_continuation) {
  GetSpellChecker().RespondToChangedSelection(old_selection_start,
                                              typing_continuation);
  Client().RespondToChangedSelection(
      &GetFrame(), GetFrame().Selection().GetSelectionInDOMTree().Type());
  SetStartNewKillRingSequence(true);
}
