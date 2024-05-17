bool FrameSelection::FrameIsFocusedAndActive() const {
  return focused_ && frame_->GetPage() &&
         frame_->GetPage()->GetFocusController().IsActive();
}
