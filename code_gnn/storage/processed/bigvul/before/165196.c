DragOperation DragController::GetDragOperation(DragData* drag_data) {
  DCHECK(drag_data);
  return drag_data->ContainsURL() && !did_initiate_drag_ ? kDragOperationCopy
                                                         : kDragOperationNone;
}
