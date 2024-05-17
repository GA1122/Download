static bool ExecuteMoveToEndOfSentenceAndModifySelection(LocalFrame& frame,
                                                         Event*,
                                                         EditorCommandSource,
                                                         const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kExtend, SelectionModifyDirection::kForward,
      TextGranularity::kSentenceBoundary, SetSelectionBy::kUser);
  return true;
}
