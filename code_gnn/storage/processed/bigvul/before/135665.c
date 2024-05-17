void FrameSelection::ScheduleVisualUpdateForPaintInvalidationIfNeeded() const {
  if (LocalFrameView* frame_view = frame_->View())
    frame_view->ScheduleVisualUpdateForPaintInvalidationIfNeeded();
}
