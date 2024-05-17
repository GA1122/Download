void SelectionController::UpdateSelectionForMouseDrag(
    Node* mouse_press_node,
    const LayoutPoint& drag_start_pos,
    const IntPoint& last_known_mouse_position) {
  LocalFrameView* view = frame_->View();
  if (!view)
    return;
  LayoutViewItem layout_item = frame_->ContentLayoutItem();
  if (layout_item.IsNull())
    return;

  HitTestRequest request(HitTestRequest::kReadOnly | HitTestRequest::kActive |
                         HitTestRequest::kMove);
  HitTestResult result(request,
                       view->RootFrameToContents(last_known_mouse_position));
  layout_item.HitTest(result);
  UpdateSelectionForMouseDrag(result, mouse_press_node, drag_start_pos,
                              last_known_mouse_position);
}
