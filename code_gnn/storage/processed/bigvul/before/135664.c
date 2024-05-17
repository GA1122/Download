void FrameSelection::ScheduleVisualUpdate() const {
  if (Page* page = frame_->GetPage())
    page->Animator().ScheduleVisualUpdate(&frame_->LocalFrameRoot());
}
