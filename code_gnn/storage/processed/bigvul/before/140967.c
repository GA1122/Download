bool Document::CanCreateHistoryEntry() const {
  if (!frame_ || frame_->HasBeenActivated())
    return true;
  if (ElapsedTime() >= kElapsedTimeForHistoryEntryWithoutUserGestureMS)
    return true;
  UseCounter::Count(*this, WebFeature::kSuppressHistoryEntryWithoutUserGesture);
  if (!GetSettings() || !GetSettings()->GetHistoryEntryRequiresUserGesture())
    return true;
  return false;
}
