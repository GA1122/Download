bool TypingCommand::insertParagraphSeparator(Document& document) {
  if (TypingCommand* lastTypingCommand =
          lastTypingCommandIfStillOpenForTyping(document.frame())) {
    lastTypingCommand->setShouldRetainAutocorrectionIndicator(false);
    EditingState editingState;
    EventQueueScope eventQueueScope;
    lastTypingCommand->insertParagraphSeparator(&editingState);
    return !editingState.isAborted();
  }

  return TypingCommand::create(document, InsertParagraphSeparator, "", 0)
      ->apply();
}
