static bool ExecuteMoveUp(LocalFrame& frame,
                          Event*,
                          EditorCommandSource,
                          const String&) {
  return frame.Selection().Modify(
      SelectionModifyAlteration::kMove, SelectionModifyDirection::kBackward,
      TextGranularity::kLine, SetSelectionBy::kUser);
}
