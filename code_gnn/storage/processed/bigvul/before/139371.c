void Editor::ReappliedEditing(UndoStep* cmd) {
  EventQueueScope scope;

  DispatchEditableContentChangedEvents(cmd->StartingRootEditableElement(),
                                       cmd->EndingRootEditableElement());
  DispatchInputEventEditableContentChanged(
      cmd->StartingRootEditableElement(), cmd->EndingRootEditableElement(),
      InputEvent::InputType::kHistoryRedo, g_null_atom,
      InputEvent::EventIsComposing::kNotComposing);

  const SelectionInDOMTree& new_selection = CorrectedSelectionAfterCommand(
      cmd->EndingSelection(), GetFrame().GetDocument());
  ChangeSelectionAfterCommand(
      new_selection,
      FrameSelection::kCloseTyping | FrameSelection::kClearTypingStyle);

  last_edit_command_ = nullptr;
  undo_stack_->RegisterUndoStep(cmd);
  RespondToChangedContents(new_selection.Base());
}
