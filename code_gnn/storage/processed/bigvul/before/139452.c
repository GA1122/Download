static bool ExecuteMoveForward(LocalFrame& frame,
                               Event*,
                               EditorCommandSource,
                               const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kMove,
                           SelectionModifyDirection::kForward,
                           TextGranularity::kCharacter, SetSelectionBy::kUser);
  return true;
}
