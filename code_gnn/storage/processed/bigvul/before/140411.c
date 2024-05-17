void TypingCommand::typingAddedToOpenCommand(
    ETypingCommand commandTypeForAddedTyping) {
  LocalFrame* frame = document().frame();
  if (!frame)
    return;

  updatePreservesTypingStyle(commandTypeForAddedTyping);
  updateCommandTypeOfOpenCommand(commandTypeForAddedTyping);

  frame->editor().appliedEditing(this);
}
