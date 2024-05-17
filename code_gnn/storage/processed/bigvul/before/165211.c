bool DragController::TryDocumentDrag(DragData* drag_data,
                                     DragDestinationAction action_mask,
                                     DragSession& drag_session,
                                     LocalFrame& local_root) {
  DCHECK(drag_data);

  if (!document_under_mouse_)
    return false;

  if (drag_initiator_ && !document_under_mouse_->GetSecurityOrigin()->CanAccess(
                             drag_initiator_->GetSecurityOrigin()))
    return false;

  bool is_handling_drag = false;
  if (action_mask & kDragDestinationActionDHTML) {
    is_handling_drag =
        TryDHTMLDrag(drag_data, drag_session.operation, local_root);
    if (!document_under_mouse_)
      return false;
  }

  LocalFrameView* frame_view = document_under_mouse_->View();
  if (!frame_view)
    return false;

  if (is_handling_drag) {
    page_->GetDragCaret().Clear();
    return true;
  }

  if ((action_mask & kDragDestinationActionEdit) &&
      CanProcessDrag(drag_data, local_root)) {
    LayoutPoint point = frame_view->RootFrameToContents(
        LayoutPoint(drag_data->ClientPosition()));
    Element* element = ElementUnderMouse(document_under_mouse_.Get(), point);
    if (!element)
      return false;

    HTMLInputElement* element_as_file_input = AsFileInput(element);
    if (file_input_element_under_mouse_ != element_as_file_input) {
      if (file_input_element_under_mouse_)
        file_input_element_under_mouse_->SetCanReceiveDroppedFiles(false);
      file_input_element_under_mouse_ = element_as_file_input;
    }

    if (!file_input_element_under_mouse_) {
      page_->GetDragCaret().SetCaretPosition(
          document_under_mouse_->GetFrame()->PositionForPoint(point));
    }

    LocalFrame* inner_frame = element->GetDocument().GetFrame();
    drag_session.operation = DragIsMove(inner_frame->Selection(), drag_data)
                                 ? kDragOperationMove
                                 : kDragOperationCopy;
    drag_session.mouse_is_over_file_input = file_input_element_under_mouse_;
    drag_session.number_of_items_to_be_accepted = 0;

    const unsigned number_of_files = drag_data->NumberOfFiles();
    if (file_input_element_under_mouse_) {
      if (file_input_element_under_mouse_->IsDisabledFormControl())
        drag_session.number_of_items_to_be_accepted = 0;
      else if (file_input_element_under_mouse_->Multiple())
        drag_session.number_of_items_to_be_accepted = number_of_files;
      else if (number_of_files == 1)
        drag_session.number_of_items_to_be_accepted = 1;
      else
        drag_session.number_of_items_to_be_accepted = 0;

      if (!drag_session.number_of_items_to_be_accepted)
        drag_session.operation = kDragOperationNone;
      file_input_element_under_mouse_->SetCanReceiveDroppedFiles(
          drag_session.number_of_items_to_be_accepted);
    } else {
      drag_session.number_of_items_to_be_accepted =
          number_of_files != 1 ? 0 : 1;
    }

    return true;
  }

  page_->GetDragCaret().Clear();
  if (file_input_element_under_mouse_)
    file_input_element_under_mouse_->SetCanReceiveDroppedFiles(false);
  file_input_element_under_mouse_ = nullptr;
  return false;
}
