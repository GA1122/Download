void NavigationControllerImpl::PruneAllButLastCommittedInternal() {
  CHECK(CanPruneAllButLastCommitted());

  entries_.erase(entries_.begin(),
                 entries_.begin() + last_committed_entry_index_);
  entries_.erase(entries_.begin() + 1, entries_.end());
  last_committed_entry_index_ = 0;
}
