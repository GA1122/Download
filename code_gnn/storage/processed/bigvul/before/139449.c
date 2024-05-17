static bool ExecuteMoveBackwardAndModifySelection(LocalFrame& frame,
                                                  Event*,
                                                  EditorCommandSource,
                                                  const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kExtend,
                           SelectionModifyDirection::kBackward,
                           TextGranularity::kCharacter, SetSelectionBy::kUser);
  return true;
}
