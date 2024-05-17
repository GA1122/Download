static bool ExecuteMoveWordRight(LocalFrame& frame,
                                 Event*,
                                 EditorCommandSource,
                                 const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kMove,
                           SelectionModifyDirection::kRight,
                           TextGranularity::kWord, SetSelectionBy::kUser);
  return true;
}
