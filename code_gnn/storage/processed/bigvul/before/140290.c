void Editor::appliedEditing(CompositeEditCommand* cmd) {
  DCHECK(!cmd->isCommandGroupWrapper());
  EventQueueScope scope;

  spellChecker().markMisspellingsAfterApplyingCommand(*cmd);

  UndoStep* undoStep = cmd->undoStep();
  DCHECK(undoStep);
  dispatchEditableContentChangedEvents(undoStep->startingRootEditableElement(),
                                       undoStep->endingRootEditableElement());
  dispatchInputEventEditableContentChanged(
      undoStep->startingRootEditableElement(),
      undoStep->endingRootEditableElement(), cmd->inputType(),
      cmd->textDataForInputEvent(), isComposingFromCommand(cmd));

  frame().document()->updateStyleAndLayoutIgnorePendingStylesheets();

  const VisibleSelection& newSelection =
      correctedVisibleSelection(cmd->endingSelection());

  changeSelectionAfterCommand(newSelection.asSelection(), 0);

  if (!cmd->preservesTypingStyle())
    clearTypingStyle();

  if (m_lastEditCommand.get() == cmd) {
    DCHECK(cmd->isTypingCommand());
  } else if (m_lastEditCommand && m_lastEditCommand->isDragAndDropCommand() &&
             (cmd->inputType() == InputEvent::InputType::DeleteByDrag ||
              cmd->inputType() == InputEvent::InputType::InsertFromDrop)) {
    if (!m_lastEditCommand->undoStep())
      m_undoStack->registerUndoStep(m_lastEditCommand->ensureUndoStep());
    m_lastEditCommand->appendCommandToUndoStep(cmd);
  } else {
    m_lastEditCommand = cmd;
    m_undoStack->registerUndoStep(m_lastEditCommand->ensureUndoStep());
  }

  respondToChangedContents(newSelection.start());
}
