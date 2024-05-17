InputEvent::EventIsComposing isComposingFromCommand(
    const CompositeEditCommand* command) {
  if (command->isTypingCommand() &&
      toTypingCommand(command)->compositionType() !=
          TypingCommand::TextCompositionNone)
    return InputEvent::EventIsComposing::IsComposing;
  return InputEvent::EventIsComposing::NotComposing;
}
