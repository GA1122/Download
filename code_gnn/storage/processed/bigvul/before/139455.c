static bool ExecuteMoveLeftAndModifySelection(LocalFrame& frame,
                                              Event*,
                                              EditorCommandSource,
                                              const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kExtend,
                           SelectionModifyDirection::kLeft,
                           TextGranularity::kCharacter, SetSelectionBy::kUser);
  return true;
}
