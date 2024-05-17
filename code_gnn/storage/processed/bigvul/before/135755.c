bool InputMethodController::ReplaceComposition(const String& text) {
  if (!HasComposition())
    return false;

  SelectComposition();

  if (GetFrame()
          .Selection()
          .ComputeVisibleSelectionInDOMTreeDeprecated()
          .IsNone())
    return false;

  if (!IsAvailable())
    return false;

  Clear();

  InsertTextDuringCompositionWithEvents(
      GetFrame(), text, 0,
      TypingCommand::TextCompositionType::kTextCompositionConfirm);
  if (!IsAvailable())
    return false;

  DispatchCompositionEndEvent(GetFrame(), text);

  return true;
}
