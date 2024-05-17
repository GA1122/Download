void TypingCommand::closeTyping(LocalFrame* frame) {
  if (TypingCommand* lastTypingCommand =
          lastTypingCommandIfStillOpenForTyping(frame))
    lastTypingCommand->closeTyping();
}
