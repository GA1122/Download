static bool ExecuteMoveToEndOfLine(LocalFrame& frame,
                                   Event*,
                                   EditorCommandSource,
                                   const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kMove, SelectionModifyDirection::kForward,
      TextGranularity::kLineBoundary, SetSelectionBy::kUser);
  return true;
}
