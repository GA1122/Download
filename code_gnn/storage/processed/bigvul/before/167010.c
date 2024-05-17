NavigationEntryImpl* NavigationControllerImpl::GetLastCommittedEntry() const {
  if (last_committed_entry_index_ == -1)
    return nullptr;
  return entries_[last_committed_entry_index_].get();
}
