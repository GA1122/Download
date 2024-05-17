String TypingCommand::textDataForInputEvent() const {
  if (m_commands.isEmpty() || isIncrementalInsertion())
    return m_textToInsert;
  return m_commands.back()->textDataForInputEvent();
}
