void FrameSelection::UpdateAppearance() {
  DCHECK(frame_->ContentLayoutObject());
  frame_caret_->ScheduleVisualUpdateForPaintInvalidationIfNeeded();
  layout_selection_->SetHasPendingSelection();
}
