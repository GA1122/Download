static bool ExecuteMoveWordLeftAndModifySelection(LocalFrame& frame,
                                                  Event*,
                                                  EditorCommandSource,
                                                  const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kExtend,
                           SelectionModifyDirection::kLeft,
                           TextGranularity::kWord, SetSelectionBy::kUser);
  return true;
}
