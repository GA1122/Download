TypingCommand* TypingCommand::lastTypingCommandIfStillOpenForTyping(
    LocalFrame* frame) {
  DCHECK(frame);

  CompositeEditCommand* lastEditCommand = frame->editor().lastEditCommand();
  if (!lastEditCommand || !lastEditCommand->isTypingCommand() ||
      !static_cast<TypingCommand*>(lastEditCommand)->isOpenForMoreTyping())
    return nullptr;

  return static_cast<TypingCommand*>(lastEditCommand);
}
