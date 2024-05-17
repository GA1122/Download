bool Editor::DeleteSelectionAfterDraggingWithEvents(
    Element* drag_source,
    DeleteMode delete_mode,
    const Position& reference_move_position) {
  if (!drag_source || !drag_source->isConnected())
    return true;

  const bool should_delete =
      DispatchBeforeInputEditorCommand(
          drag_source, InputEvent::InputType::kDeleteByDrag,
          TargetRangesForInputEvent(*drag_source)) ==
      DispatchEventResult::kNotCanceled;

  if (frame_->GetDocument()->GetFrame() != frame_)
    return false;

  if (should_delete && drag_source->isConnected()) {
    DeleteSelectionWithSmartDelete(delete_mode,
                                   InputEvent::InputType::kDeleteByDrag,
                                   reference_move_position);
  }

  return true;
}
