static DragOperation DefaultOperationForDrag(DragOperation src_op_mask) {
  if (src_op_mask == kDragOperationEvery)
    return kDragOperationCopy;
  if (src_op_mask == kDragOperationNone)
    return kDragOperationNone;
  if (src_op_mask & kDragOperationMove)
    return kDragOperationMove;
  if (src_op_mask & kDragOperationCopy)
    return kDragOperationCopy;
  if (src_op_mask & kDragOperationLink)
    return kDragOperationLink;

  return kDragOperationGeneric;
}
