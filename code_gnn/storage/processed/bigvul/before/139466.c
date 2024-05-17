static bool ExecuteMoveToBeginningOfLineAndModifySelection(LocalFrame& frame,
                                                           Event*,
                                                           EditorCommandSource,
                                                           const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kExtend, SelectionModifyDirection::kBackward,
      TextGranularity::kLineBoundary, SetSelectionBy::kUser);
  return true;
}
