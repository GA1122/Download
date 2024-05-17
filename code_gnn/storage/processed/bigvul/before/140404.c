void TypingCommand::insertParagraphSeparatorInQuotedContent(
    EditingState* editingState) {
  if (enclosingNodeOfType(endingSelection().start(), &isTableStructureNode)) {
    insertParagraphSeparator(editingState);
    return;
  }

  applyCommandToComposite(BreakBlockquoteCommand::create(document()),
                          editingState);
  if (editingState->isAborted())
    return;
  typingAddedToOpenCommand(InsertParagraphSeparatorInQuotedContent);
}
