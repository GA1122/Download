void NavigationControllerImpl::DiscardNonCommittedEntries() {
  bool transient = transient_entry_index_ != -1;
  DiscardNonCommittedEntriesInternal();

  if (transient) {
    web_contents_->NotifyNavigationStateChanged(kInvalidateAll);
  }
}
