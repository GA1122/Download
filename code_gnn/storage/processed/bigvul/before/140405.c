void TypingCommand::insertText(const String& text,
                               bool selectInsertedText,
                               EditingState* editingState) {
  m_textToInsert = text;

  if (text.isEmpty()) {
    insertTextRunWithoutNewlines(text, selectInsertedText, editingState);
    return;
  }
  unsigned offset = 0;
  size_t newline;
  while ((newline = text.find('\n', offset)) != kNotFound) {
    if (newline > offset) {
      const size_t insertionLength = newline - offset;
      insertTextRunWithoutNewlines(text.substring(offset, insertionLength),
                                   false, editingState);
      if (editingState->isAborted())
        return;

      adjustSelectionAfterIncrementalInsertion(document().frame(),
                                               insertionLength);
    }

    insertParagraphSeparator(editingState);
    if (editingState->isAborted())
      return;

    offset = newline + 1;
  }

  if (!offset) {
    insertTextRunWithoutNewlines(text, selectInsertedText, editingState);
    if (editingState->isAborted())
      return;

    adjustSelectionAfterIncrementalInsertion(document().frame(), text.length());
    return;
  }

  if (text.length() > offset) {
    const size_t insertionLength = text.length() - offset;
    insertTextRunWithoutNewlines(text.substring(offset, insertionLength),
                                 selectInsertedText, editingState);
    if (editingState->isAborted())
      return;

    adjustSelectionAfterIncrementalInsertion(document().frame(),
                                             insertionLength);
  }
}
