static bool ExecuteMoveParagraphForward(LocalFrame& frame,
                                        Event*,
                                        EditorCommandSource,
                                        const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kMove,
                           SelectionModifyDirection::kForward,
                           TextGranularity::kParagraph, SetSelectionBy::kUser);
  return true;
}
