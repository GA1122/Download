void FrameSelection::UpdateAppearance() {
  DCHECK(!frame_->ContentLayoutItem().IsNull());
  frame_caret_->ScheduleVisualUpdateForPaintInvalidationIfNeeded();
  layout_selection_->SetHasPendingSelection();
}
