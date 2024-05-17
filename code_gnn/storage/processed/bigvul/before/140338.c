bool Editor::insertParagraphSeparator() {
  if (!canEdit())
    return false;

  if (!canEditRichly())
    return insertLineBreak();

  VisiblePosition caret = frame()
                              .selection()
                              .computeVisibleSelectionInDOMTreeDeprecated()
                              .visibleStart();
  bool alignToEdge = isEndOfEditableOrNonEditableContent(caret);
  DCHECK(frame().document());
  EditingState editingState;
  if (!TypingCommand::insertParagraphSeparator(*frame().document()))
    return false;
  revealSelectionAfterEditingOperation(
      alignToEdge ? ScrollAlignment::alignToEdgeIfNeeded
                  : ScrollAlignment::alignCenterIfNeeded);

  return true;
}
