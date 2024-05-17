void InputMethodController::CancelComposition() {
  if (!HasComposition())
    return;

  Editor::RevealSelectionScope reveal_selection_scope(&GetEditor());

  if (GetFrame()
          .Selection()
          .ComputeVisibleSelectionInDOMTreeDeprecated()
          .IsNone())
    return;

  Clear();

  InsertTextDuringCompositionWithEvents(
      GetFrame(), g_empty_string, 0,
      TypingCommand::TextCompositionType::kTextCompositionCancel);
  if (!IsAvailable())
    return;

  TypingCommand::CloseTyping(frame_);

  DispatchCompositionEndEvent(GetFrame(), g_empty_string);
}
