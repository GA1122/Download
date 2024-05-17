mojom::PageVisibilityState Document::GetPageVisibilityState() const {
  if (!frame_ || !frame_->GetPage())
    return mojom::PageVisibilityState::kHidden;
  if (load_event_progress_ >= kUnloadVisibilityChangeInProgress)
    return mojom::PageVisibilityState::kHidden;
  return frame_->GetPage()->VisibilityState();
}
