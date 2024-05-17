static bool ExecuteMoveToEndOfParagraph(LocalFrame& frame,
                                        Event*,
                                        EditorCommandSource,
                                        const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kMove, SelectionModifyDirection::kForward,
      TextGranularity::kParagraphBoundary, SetSelectionBy::kUser);
  return true;
}
