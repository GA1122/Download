static bool ExecuteMoveToLeftEndOfLineAndModifySelection(LocalFrame& frame,
                                                         Event*,
                                                         EditorCommandSource,
                                                         const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kExtend, SelectionModifyDirection::kLeft,
      TextGranularity::kLineBoundary, SetSelectionBy::kUser);
  return true;
}
