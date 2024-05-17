PageVisibilityState Document::GetPageVisibilityState() const {
  if (!frame_ || !frame_->GetPage())
    return kPageVisibilityStateHidden;
  if (load_event_progress_ >= kUnloadVisibilityChangeInProgress)
    return kPageVisibilityStateHidden;
  return frame_->GetPage()->VisibilityState();
}
