void NavigationControllerImpl::DiscardNonCommittedEntries() {
  bool transient = transient_entry_index_ != -1;
  DiscardNonCommittedEntriesInternal();

  if (transient) {
    delegate_->NotifyNavigationStateChanged(INVALIDATE_TYPE_ALL);
  }
}
