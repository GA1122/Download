static bool ExecuteMoveToEndOfDocumentAndModifySelection(LocalFrame& frame,
                                                         Event*,
                                                         EditorCommandSource,
                                                         const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kExtend, SelectionModifyDirection::kForward,
      TextGranularity::kDocumentBoundary, SetSelectionBy::kUser);
  return true;
}
