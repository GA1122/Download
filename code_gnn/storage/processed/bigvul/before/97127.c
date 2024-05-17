void NavigationController::NavigateToPendingEntry(bool reload) {
  if (!pending_entry_) {
    DCHECK(pending_entry_index_ != -1);
    pending_entry_ = entries_[pending_entry_index_].get();
  }

  if (!tab_contents_->NavigateToPendingEntry(reload))
    DiscardNonCommittedEntries();
}
