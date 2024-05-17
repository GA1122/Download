static bool ExecuteMoveDownAndModifySelection(LocalFrame& frame,
                                              Event*,
                                              EditorCommandSource,
                                              const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kExtend,
                           SelectionModifyDirection::kForward,
                           TextGranularity::kLine, SetSelectionBy::kUser);
  return true;
}
