static bool ExecuteMoveToBeginningOfParagraphAndModifySelection(
    LocalFrame& frame,
    Event*,
    EditorCommandSource,
    const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kExtend, SelectionModifyDirection::kBackward,
      TextGranularity::kParagraphBoundary, SetSelectionBy::kUser);
  return true;
}
