TypingCommand* Editor::LastTypingCommandIfStillOpenForTyping() const {
  return TypingCommand::LastTypingCommandIfStillOpenForTyping(&GetFrame());
}
