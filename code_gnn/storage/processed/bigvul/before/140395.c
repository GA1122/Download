void TypingCommand::forwardDeleteKeyPressed(Document& document,
                                            EditingState* editingState,
                                            Options options,
                                            TextGranularity granularity) {
  if (granularity == CharacterGranularity) {
    LocalFrame* frame = document.frame();
    if (TypingCommand* lastTypingCommand =
            lastTypingCommandIfStillOpenForTyping(frame)) {
      updateSelectionIfDifferentFromCurrentSelection(lastTypingCommand, frame);
      lastTypingCommand->setShouldPreventSpellChecking(options &
                                                       PreventSpellChecking);
      lastTypingCommand->forwardDeleteKeyPressed(
          granularity, options & KillRing, editingState);
      return;
    }
  }

  TypingCommand::create(document, ForwardDeleteKey, "", options, granularity)
      ->apply();
}
