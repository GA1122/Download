bool Editor::ReplaceSelectionAfterDraggingWithEvents(
    Element* drop_target,
    DragData* drag_data,
    DocumentFragment* fragment,
    Range* drop_caret_range,
    InsertMode insert_mode,
    DragSourceType drag_source_type) {
  if (!drop_target || !drop_target->isConnected())
    return true;

  DataTransfer* data_transfer =
      DataTransfer::Create(DataTransfer::kDragAndDrop, kDataTransferReadable,
                           drag_data->PlatformData());
  data_transfer->SetSourceOperation(drag_data->DraggingSourceOperationMask());
  const bool should_insert =
      DispatchBeforeInputDataTransfer(
          drop_target, InputEvent::InputType::kInsertFromDrop, data_transfer) ==
      DispatchEventResult::kNotCanceled;

  if (frame_->GetDocument()->GetFrame() != frame_)
    return false;

  if (should_insert && drop_target->isConnected())
    ReplaceSelectionAfterDragging(fragment, insert_mode, drag_source_type);

  return true;
}
