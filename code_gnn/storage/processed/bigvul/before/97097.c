bool NavigationController::CanGoBack() const {
  return entries_.size() > 1 && GetCurrentEntryIndex() > 0;
}