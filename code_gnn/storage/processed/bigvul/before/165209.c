void DragController::Trace(blink::Visitor* visitor) {
  visitor->Trace(page_);
  visitor->Trace(document_under_mouse_);
  visitor->Trace(drag_initiator_);
  visitor->Trace(drag_state_);
  visitor->Trace(file_input_element_under_mouse_);
}
