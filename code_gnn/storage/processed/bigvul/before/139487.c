static bool ExecuteMoveWordLeft(LocalFrame& frame,
                                Event*,
                                EditorCommandSource,
                                const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kMove,
                           SelectionModifyDirection::kLeft,
                           TextGranularity::kWord, SetSelectionBy::kUser);
  return true;
}
