bool TypingCommand::insertLineBreak(Document& document) {
  if (TypingCommand* lastTypingCommand =
          lastTypingCommandIfStillOpenForTyping(document.frame())) {
    lastTypingCommand->setShouldRetainAutocorrectionIndicator(false);
    EditingState editingState;
    EventQueueScope eventQueueScope;
    lastTypingCommand->insertLineBreak(&editingState);
    return !editingState.isAborted();
  }

  return TypingCommand::create(document, InsertLineBreak, "", 0)->apply();
}
