static bool ExecuteMoveToRightEndOfLineAndModifySelection(LocalFrame& frame,
                                                          Event*,
                                                          EditorCommandSource,
                                                          const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kExtend, SelectionModifyDirection::kRight,
      TextGranularity::kLineBoundary, SetSelectionBy::kUser);
  return true;
}
