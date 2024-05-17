void TypingCommand::insertParagraphSeparator(EditingState* editingState) {
  if (!canAppendNewLineFeedToSelection(endingSelection()))
    return;

  applyCommandToComposite(InsertParagraphSeparatorCommand::create(document()),
                          editingState);
  if (editingState->isAborted())
    return;
  typingAddedToOpenCommand(InsertParagraphSeparator);
}
