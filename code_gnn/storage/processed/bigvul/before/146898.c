bool Document::hidden() const {
  return GetPageVisibilityState() != kPageVisibilityStateVisible;
}
