int BrowserTabStripController::HasAvailableDragActions() const {
  return model_->delegate()->GetDragActions();
}
