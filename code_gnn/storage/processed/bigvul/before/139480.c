static bool ExecuteMoveToRightEndOfLine(LocalFrame& frame,
                                        Event*,
                                        EditorCommandSource,
                                        const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kMove, SelectionModifyDirection::kRight,
      TextGranularity::kLineBoundary, SetSelectionBy::kUser);
  return true;
}
