void SelectionController::HandleMouseDraggedEvent(
    const MouseEventWithHitTestResults& event,
    const IntPoint& mouse_down_pos,
    const LayoutPoint& drag_start_pos,
    Node* mouse_press_node,
    const IntPoint& last_known_mouse_position) {
  TRACE_EVENT0("blink", "SelectionController::handleMouseDraggedEvent");

  if (!Selection().IsAvailable())
    return;
  if (selection_state_ != SelectionState::kExtendedSelection) {
    HitTestRequest request(HitTestRequest::kReadOnly | HitTestRequest::kActive);
    HitTestResult result(request, mouse_down_pos);
    frame_->GetDocument()->GetLayoutViewItem().HitTest(result);

    UpdateSelectionForMouseDrag(result, mouse_press_node, drag_start_pos,
                                last_known_mouse_position);
  }
  UpdateSelectionForMouseDrag(event.GetHitTestResult(), mouse_press_node,
                              drag_start_pos, last_known_mouse_position);
}
