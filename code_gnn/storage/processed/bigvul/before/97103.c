void NavigationController::DiscardNonCommittedEntries() {
  bool transient = transient_entry_index_ != -1;
  DiscardNonCommittedEntriesInternal();

  if (transient) {
    tab_contents_->NotifyNavigationStateChanged(kInvalidateAllButShelves);
  }
}
