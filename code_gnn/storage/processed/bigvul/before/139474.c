static bool ExecuteMoveToEndOfLineAndModifySelection(LocalFrame& frame,
                                                     Event*,
                                                     EditorCommandSource,
                                                     const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kExtend, SelectionModifyDirection::kForward,
      TextGranularity::kLineBoundary, SetSelectionBy::kUser);
  return true;
}
