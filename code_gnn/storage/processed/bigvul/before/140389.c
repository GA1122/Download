void TypingCommand::deleteSelection(Document& document, Options options) {
  LocalFrame* frame = document.frame();
  DCHECK(frame);

  if (!frame->selection()
           .computeVisibleSelectionInDOMTreeDeprecated()
           .isRange())
    return;

  if (TypingCommand* lastTypingCommand =
          lastTypingCommandIfStillOpenForTyping(frame)) {
    updateSelectionIfDifferentFromCurrentSelection(lastTypingCommand, frame);

    lastTypingCommand->setShouldPreventSpellChecking(options &
                                                     PreventSpellChecking);
    lastTypingCommand->deleteSelection(options & SmartDelete,
                                       ASSERT_NO_EDITING_ABORT);
    return;
  }

  TypingCommand::create(document, DeleteSelection, "", options)->apply();
}
