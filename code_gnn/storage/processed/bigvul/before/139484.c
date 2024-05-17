static bool ExecuteMoveWordBackward(LocalFrame& frame,
                                    Event*,
                                    EditorCommandSource,
                                    const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kMove,
                           SelectionModifyDirection::kBackward,
                           TextGranularity::kWord, SetSelectionBy::kUser);
  return true;
}
