void Editor::ReplaceSelectionAfterDragging(DocumentFragment* fragment,
                                           InsertMode insert_mode,
                                           DragSourceType drag_source_type) {
  ReplaceSelectionCommand::CommandOptions options =
      ReplaceSelectionCommand::kSelectReplacement |
      ReplaceSelectionCommand::kPreventNesting;
  if (insert_mode == InsertMode::kSmart)
    options |= ReplaceSelectionCommand::kSmartReplace;
  if (drag_source_type == DragSourceType::kPlainTextSource)
    options |= ReplaceSelectionCommand::kMatchStyle;
  DCHECK(GetFrame().GetDocument());
  ReplaceSelectionCommand::Create(*GetFrame().GetDocument(), fragment, options,
                                  InputEvent::InputType::kInsertFromDrop)
      ->Apply();
}
