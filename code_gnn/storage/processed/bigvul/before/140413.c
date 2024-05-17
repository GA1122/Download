void TypingCommand::updateSelectionIfDifferentFromCurrentSelection(
    TypingCommand* typingCommand,
    LocalFrame* frame) {
  DCHECK(frame);
  VisibleSelection currentSelection =
      frame->selection().computeVisibleSelectionInDOMTreeDeprecated();
  if (currentSelection == typingCommand->endingSelection())
    return;

  typingCommand->setStartingSelection(currentSelection);
  typingCommand->setEndingVisibleSelection(currentSelection);
}
