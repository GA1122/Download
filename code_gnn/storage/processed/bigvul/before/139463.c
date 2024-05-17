static bool ExecuteMoveParagraphForwardAndModifySelection(LocalFrame& frame,
                                                          Event*,
                                                          EditorCommandSource,
                                                          const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kExtend,
                           SelectionModifyDirection::kForward,
                           TextGranularity::kParagraph, SetSelectionBy::kUser);
  return true;
}
