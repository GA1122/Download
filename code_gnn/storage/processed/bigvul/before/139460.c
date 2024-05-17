static bool ExecuteMoveParagraphBackward(LocalFrame& frame,
                                         Event*,
                                         EditorCommandSource,
                                         const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kMove,
                           SelectionModifyDirection::kBackward,
                           TextGranularity::kParagraph, SetSelectionBy::kUser);
  return true;
}
