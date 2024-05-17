InputEvent::EventIsComposing IsComposingFromCommand(
    const CompositeEditCommand* command) {
  if (command->IsTypingCommand() &&
      ToTypingCommand(command)->CompositionType() !=
          TypingCommand::kTextCompositionNone)
    return InputEvent::EventIsComposing::kIsComposing;
  return InputEvent::EventIsComposing::kNotComposing;
}
