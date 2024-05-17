bool Document::IsLoadCompleted() {
  return ready_state_ == kComplete;
}
