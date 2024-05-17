void Editor::replaceSelectionAfterDragging(DocumentFragment* fragment,
                                           InsertMode insertMode,
                                           DragSourceType dragSourceType) {
  ReplaceSelectionCommand::CommandOptions options =
      ReplaceSelectionCommand::SelectReplacement |
      ReplaceSelectionCommand::PreventNesting;
  if (insertMode == InsertMode::Smart)
    options |= ReplaceSelectionCommand::SmartReplace;
  if (dragSourceType == DragSourceType::PlainTextSource)
    options |= ReplaceSelectionCommand::MatchStyle;
  DCHECK(frame().document());
  ReplaceSelectionCommand::create(*frame().document(), fragment, options,
                                  InputEvent::InputType::InsertFromDrop)
      ->apply();
}
