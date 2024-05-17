static bool ExecuteMoveDown(LocalFrame& frame,
                            Event*,
                            EditorCommandSource,
                            const String&) {
  return frame.Selection().Modify(
      SelectionModifyAlteration::kMove, SelectionModifyDirection::kForward,
      TextGranularity::kLine, SetSelectionBy::kUser);
}
