bool Editor::insertLineBreak() {
  if (!canEdit())
    return false;

  VisiblePosition caret = frame()
                              .selection()
                              .computeVisibleSelectionInDOMTreeDeprecated()
                              .visibleStart();
  bool alignToEdge = isEndOfEditableOrNonEditableContent(caret);
  DCHECK(frame().document());
  if (!TypingCommand::insertLineBreak(*frame().document()))
    return false;
  revealSelectionAfterEditingOperation(
      alignToEdge ? ScrollAlignment::alignToEdgeIfNeeded
                  : ScrollAlignment::alignCenterIfNeeded);

  return true;
}
