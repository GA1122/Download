void InputMethodController::ExtendSelectionAndDelete(int before, int after) {
  if (!GetEditor().CanEdit())
    return;
  PlainTextRange selection_offsets(GetSelectionOffsets());
  if (selection_offsets.IsNull())
    return;

  do {
    if (!SetSelectionOffsets(PlainTextRange(
            std::max(static_cast<int>(selection_offsets.Start()) - before, 0),
            selection_offsets.End() + after)))
      return;
    if (before == 0)
      break;
    ++before;
  } while (GetFrame()
                   .Selection()
                   .ComputeVisibleSelectionInDOMTreeDeprecated()
                   .Start() == GetFrame()
                                   .Selection()
                                   .ComputeVisibleSelectionInDOMTreeDeprecated()
                                   .End() &&
           before <= static_cast<int>(selection_offsets.Start()));
  Node* target = GetDocument().FocusedElement();
  if (target) {
    DispatchBeforeInputEditorCommand(
        target, InputEvent::InputType::kDeleteContentBackward,
        TargetRangesForInputEvent(*target));
  }
  TypingCommand::DeleteSelection(GetDocument());
}
