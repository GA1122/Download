static bool ExecuteMoveForwardAndModifySelection(LocalFrame& frame,
                                                 Event*,
                                                 EditorCommandSource,
                                                 const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kExtend,
                           SelectionModifyDirection::kForward,
                           TextGranularity::kCharacter, SetSelectionBy::kUser);
  return true;
}
