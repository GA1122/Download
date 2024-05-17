void NavigationControllerImpl::PruneAllButVisibleInternal() {
  CHECK(CanPruneAllButVisible());

  entries_.erase(entries_.begin(),
                 entries_.begin() + last_committed_entry_index_);
  entries_.erase(entries_.begin() + 1, entries_.end());
  last_committed_entry_index_ = 0;
}
