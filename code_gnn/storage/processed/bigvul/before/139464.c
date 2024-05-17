static bool ExecuteMoveRight(LocalFrame& frame,
                             Event*,
                             EditorCommandSource,
                             const String&) {
  return frame.Selection().Modify(
      SelectionModifyAlteration::kMove, SelectionModifyDirection::kRight,
      TextGranularity::kCharacter, SetSelectionBy::kUser);
}
