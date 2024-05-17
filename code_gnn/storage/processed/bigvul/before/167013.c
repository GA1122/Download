NavigationEntryImpl* NavigationControllerImpl::GetTransientEntry() const {
  if (transient_entry_index_ == -1)
    return nullptr;
  return entries_[transient_entry_index_].get();
}
