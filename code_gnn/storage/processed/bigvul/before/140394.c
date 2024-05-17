void TypingCommand::doApply(EditingState* editingState) {
  if (!endingSelection().isNonOrphanedCaretOrRange())
    return;

  if (m_commandType == DeleteKey) {
    if (m_commands.isEmpty())
      m_openedByBackwardDelete = true;
  }

  switch (m_commandType) {
    case DeleteSelection:
      deleteSelection(m_smartDelete, editingState);
      return;
    case DeleteKey:
      deleteKeyPressed(m_granularity, m_killRing, editingState);
      return;
    case ForwardDeleteKey:
      forwardDeleteKeyPressed(m_granularity, m_killRing, editingState);
      return;
    case InsertLineBreak:
      insertLineBreak(editingState);
      return;
    case InsertParagraphSeparator:
      insertParagraphSeparator(editingState);
      return;
    case InsertParagraphSeparatorInQuotedContent:
      insertParagraphSeparatorInQuotedContent(editingState);
      return;
    case InsertText:
      insertText(m_textToInsert, m_selectInsertedText, editingState);
      return;
  }

  NOTREACHED();
}
