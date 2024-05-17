DragSession DragController::DragEnteredOrUpdated(DragData* drag_data,
                                                 LocalFrame& local_root) {
  DCHECK(drag_data);

  MouseMovedIntoDocument(
      local_root.DocumentAtPoint(LayoutPoint(drag_data->ClientPosition())));

  drag_destination_action_ =
      page_->GetChromeClient().AcceptsLoadDrops()
          ? kDragDestinationActionAny
          : static_cast<DragDestinationAction>(kDragDestinationActionDHTML |
                                               kDragDestinationActionEdit);

  DragSession drag_session;
  document_is_handling_drag_ = TryDocumentDrag(
      drag_data, drag_destination_action_, drag_session, local_root);
  if (!document_is_handling_drag_ &&
      (drag_destination_action_ & kDragDestinationActionLoad))
    drag_session.operation = OperationForLoad(drag_data, local_root);
  return drag_session;
}
