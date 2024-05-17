static bool ExecuteMoveToLeftEndOfLine(LocalFrame& frame,
                                       Event*,
                                       EditorCommandSource,
                                       const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kMove, SelectionModifyDirection::kLeft,
      TextGranularity::kLineBoundary, SetSelectionBy::kUser);
  return true;
}
