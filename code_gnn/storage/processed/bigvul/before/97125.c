void NavigationController::LoadIfNecessary() {
  if (!needs_reload_)
    return;

  needs_reload_ = false;
  pending_entry_index_ = last_committed_entry_index_;
  NavigateToPendingEntry(false);
}
