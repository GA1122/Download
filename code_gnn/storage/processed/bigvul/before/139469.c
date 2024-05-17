static bool ExecuteMoveToBeginningOfSentence(LocalFrame& frame,
                                             Event*,
                                             EditorCommandSource,
                                             const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kMove, SelectionModifyDirection::kBackward,
      TextGranularity::kSentenceBoundary, SetSelectionBy::kUser);
  return true;
}
