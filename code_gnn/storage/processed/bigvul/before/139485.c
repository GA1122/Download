static bool ExecuteMoveWordBackwardAndModifySelection(LocalFrame& frame,
                                                      Event*,
                                                      EditorCommandSource,
                                                      const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kExtend,
                           SelectionModifyDirection::kBackward,
                           TextGranularity::kWord, SetSelectionBy::kUser);
  return true;
}
