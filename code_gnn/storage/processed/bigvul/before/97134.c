void NavigationController::RemoveEntryAtIndex(int index,
                                              const GURL& default_url) {
  int size = static_cast<int>(entries_.size());
  DCHECK(index < size);

  DiscardNonCommittedEntries();

  entries_.erase(entries_.begin() + index);

  if (last_committed_entry_index_ == index) {
    last_committed_entry_index_--;
    if (last_committed_entry_index_ != -1) {
      pending_entry_index_ = last_committed_entry_index_;
      NavigateToPendingEntry(false);
    } else {
      LoadURL(default_url.is_empty() ? GURL("about:blank") : default_url,
              GURL(), PageTransition::START_PAGE);
    }
  } else if (last_committed_entry_index_ > index) {
    last_committed_entry_index_--;
  }
}
