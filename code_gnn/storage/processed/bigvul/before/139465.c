static bool ExecuteMoveRightAndModifySelection(LocalFrame& frame,
                                               Event*,
                                               EditorCommandSource,
                                               const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kExtend,
                           SelectionModifyDirection::kRight,
                           TextGranularity::kCharacter, SetSelectionBy::kUser);
  return true;
}
