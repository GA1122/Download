void TypingCommand::deleteSelection(bool smartDelete,
                                    EditingState* editingState) {
  CompositeEditCommand::deleteSelection(editingState, smartDelete);
  if (editingState->isAborted())
    return;
  typingAddedToOpenCommand(DeleteSelection);
}
