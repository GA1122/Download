TypingCommand* Editor::lastTypingCommandIfStillOpenForTyping() const {
  return TypingCommand::lastTypingCommandIfStillOpenForTyping(&frame());
}
