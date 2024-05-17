static bool ExecuteMoveToBeginningOfParagraph(LocalFrame& frame,
                                              Event*,
                                              EditorCommandSource,
                                              const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kMove, SelectionModifyDirection::kBackward,
      TextGranularity::kParagraphBoundary, SetSelectionBy::kUser);
  return true;
}
