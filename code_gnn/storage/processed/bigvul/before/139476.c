static bool ExecuteMoveToEndOfParagraphAndModifySelection(LocalFrame& frame,
                                                          Event*,
                                                          EditorCommandSource,
                                                          const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kExtend, SelectionModifyDirection::kForward,
      TextGranularity::kParagraphBoundary, SetSelectionBy::kUser);
  return true;
}
