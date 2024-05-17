bool TypingCommand::insertParagraphSeparatorInQuotedContent(
    Document& document) {
  if (TypingCommand* lastTypingCommand =
          lastTypingCommandIfStillOpenForTyping(document.frame())) {
    EditingState editingState;
    EventQueueScope eventQueueScope;
    lastTypingCommand->insertParagraphSeparatorInQuotedContent(&editingState);
    return !editingState.isAborted();
  }

  return TypingCommand::create(document,
                               InsertParagraphSeparatorInQuotedContent)
      ->apply();
}
