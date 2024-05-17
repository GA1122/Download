static bool ExecuteMoveToEndOfDocument(LocalFrame& frame,
                                       Event*,
                                       EditorCommandSource,
                                       const String&) {
  frame.Selection().Modify(
      SelectionModifyAlteration::kMove, SelectionModifyDirection::kForward,
      TextGranularity::kDocumentBoundary, SetSelectionBy::kUser);
  return true;
}
