void DragController::DragEnded() {
  drag_initiator_ = nullptr;
  did_initiate_drag_ = false;
  page_->GetDragCaret().Clear();
}
