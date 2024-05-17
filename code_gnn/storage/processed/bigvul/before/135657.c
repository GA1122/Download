void FrameSelection::NotifyEventHandlerForSelectionChange() {
  frame_->GetEventHandler().GetSelectionController().NotifySelectionChanged();
}
