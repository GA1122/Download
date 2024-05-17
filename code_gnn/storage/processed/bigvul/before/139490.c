static bool ExecuteMoveWordRightAndModifySelection(LocalFrame& frame,
                                                   Event*,
                                                   EditorCommandSource,
                                                   const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kExtend,
                           SelectionModifyDirection::kRight,
                           TextGranularity::kWord, SetSelectionBy::kUser);
  return true;
}
