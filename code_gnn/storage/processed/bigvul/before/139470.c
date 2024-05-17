static bool ExecuteMoveToBeginningOfSentenceAndModifySelection(
    LocalFrame& frame,
    Event*,
    EditorCommandSource,
    const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kExtend, SelectionModifyDirection::kBackward,
      TextGranularity::kSentenceBoundary, SetSelectionBy::kUser);
  return true;
}
