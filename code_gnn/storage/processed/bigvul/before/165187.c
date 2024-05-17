void DragController::DragExited(DragData* drag_data, LocalFrame& local_root) {
  DCHECK(drag_data);

  LocalFrameView* frame_view(local_root.View());
  if (frame_view) {
    DataTransferAccessPolicy policy = kDataTransferTypesReadable;
    DataTransfer* data_transfer = CreateDraggingDataTransfer(policy, drag_data);
    data_transfer->SetSourceOperation(drag_data->DraggingSourceOperationMask());
    local_root.GetEventHandler().CancelDragAndDrop(CreateMouseEvent(drag_data),
                                                   data_transfer);
    data_transfer->SetAccessPolicy(
        kDataTransferNumb);   
  }
  MouseMovedIntoDocument(nullptr);
  if (file_input_element_under_mouse_)
    file_input_element_under_mouse_->SetCanReceiveDroppedFiles(false);
  file_input_element_under_mouse_ = nullptr;
}
