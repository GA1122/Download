bool Document::IsLoadCompleted() const {
  return ready_state_ == kComplete;
}
