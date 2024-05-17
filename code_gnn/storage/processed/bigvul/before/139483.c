static bool ExecuteMoveUpAndModifySelection(LocalFrame& frame,
                                            Event*,
                                            EditorCommandSource,
                                            const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kExtend,
                           SelectionModifyDirection::kBackward,
                           TextGranularity::kLine, SetSelectionBy::kUser);
  return true;
}
