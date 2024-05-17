bool DragController::TryDHTMLDrag(DragData* drag_data,
                                  DragOperation& operation,
                                  LocalFrame& local_root) {
  DCHECK(drag_data);
  DCHECK(document_under_mouse_);
  if (!local_root.View())
    return false;

  DataTransferAccessPolicy policy = kDataTransferTypesReadable;
  DataTransfer* data_transfer = CreateDraggingDataTransfer(policy, drag_data);
  DragOperation src_op_mask = drag_data->DraggingSourceOperationMask();
  data_transfer->SetSourceOperation(src_op_mask);

  WebMouseEvent event = CreateMouseEvent(drag_data);
  if (local_root.GetEventHandler().UpdateDragAndDrop(event, data_transfer) ==
      WebInputEventResult::kNotHandled) {
    data_transfer->SetAccessPolicy(
        kDataTransferNumb);   
    return false;
  }

  operation = data_transfer->DestinationOperation();
  if (data_transfer->DropEffectIsUninitialized()) {
    operation = DefaultOperationForDrag(src_op_mask);
  } else if (!(src_op_mask & operation)) {
    operation = kDragOperationNone;
  }

  data_transfer->SetAccessPolicy(
      kDataTransferNumb);   
  return true;
}
