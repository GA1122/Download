void Editor::reappliedEditing(UndoStep* cmd) {
  EventQueueScope scope;

  dispatchEditableContentChangedEvents(cmd->startingRootEditableElement(),
                                       cmd->endingRootEditableElement());
  dispatchInputEventEditableContentChanged(
      cmd->startingRootEditableElement(), cmd->endingRootEditableElement(),
      InputEvent::InputType::HistoryRedo, nullAtom,
      InputEvent::EventIsComposing::NotComposing);

  frame().document()->updateStyleAndLayoutIgnorePendingStylesheets();
  const VisibleSelection& newSelection =
      correctedVisibleSelection(cmd->endingSelection());
  DCHECK(newSelection.isValidFor(*frame().document())) << newSelection;
  changeSelectionAfterCommand(
      newSelection.asSelection(),
      FrameSelection::CloseTyping | FrameSelection::ClearTypingStyle);

  m_lastEditCommand = nullptr;
  m_undoStack->registerUndoStep(cmd);
  respondToChangedContents(newSelection.start());
}
