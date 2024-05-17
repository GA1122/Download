void TypingCommand::deleteKeyPressed(Document& document,
                                     Options options,
                                     TextGranularity granularity) {
  if (granularity == CharacterGranularity) {
    LocalFrame* frame = document.frame();
    if (TypingCommand* lastTypingCommand =
            lastTypingCommandIfStillOpenForTyping(frame)) {
      if (lastTypingCommand->commandTypeOfOpenCommand() == DeleteKey) {
        updateSelectionIfDifferentFromCurrentSelection(lastTypingCommand,
                                                       frame);
        lastTypingCommand->setShouldPreventSpellChecking(options &
                                                         PreventSpellChecking);
        EditingState editingState;
        lastTypingCommand->deleteKeyPressed(granularity, options & KillRing,
                                            &editingState);
        return;
      }
    }
  }

  TypingCommand::create(document, DeleteKey, "", options, granularity)->apply();
}
