void Editor::AppliedEditing(CompositeEditCommand* cmd) {
  DCHECK(!cmd->IsCommandGroupWrapper());
  EventQueueScope scope;

  GetSpellChecker().MarkMisspellingsAfterApplyingCommand(*cmd);

  UndoStep* undo_step = cmd->GetUndoStep();
  DCHECK(undo_step);
  DispatchEditableContentChangedEvents(undo_step->StartingRootEditableElement(),
                                       undo_step->EndingRootEditableElement());
  DispatchInputEventEditableContentChanged(
      undo_step->StartingRootEditableElement(),
      undo_step->EndingRootEditableElement(), cmd->GetInputType(),
      cmd->TextDataForInputEvent(), IsComposingFromCommand(cmd));

  const SelectionInDOMTree& new_selection = CorrectedSelectionAfterCommand(
      cmd->EndingSelection(), GetFrame().GetDocument());

  ChangeSelectionAfterCommand(new_selection, 0);

  if (!cmd->PreservesTypingStyle())
    ClearTypingStyle();

  if (last_edit_command_.Get() == cmd) {
    DCHECK(cmd->IsTypingCommand());
  } else if (last_edit_command_ && last_edit_command_->IsDragAndDropCommand() &&
             (cmd->GetInputType() == InputEvent::InputType::kDeleteByDrag ||
              cmd->GetInputType() == InputEvent::InputType::kInsertFromDrop)) {
    if (!last_edit_command_->GetUndoStep())
      undo_stack_->RegisterUndoStep(last_edit_command_->EnsureUndoStep());
    last_edit_command_->EnsureUndoStep()->SetEndingSelection(
        cmd->EnsureUndoStep()->EndingSelection());
    last_edit_command_->AppendCommandToUndoStep(cmd);
  } else {
    last_edit_command_ = cmd;
    undo_stack_->RegisterUndoStep(last_edit_command_->EnsureUndoStep());
  }

  RespondToChangedContents(new_selection.Base());
}