void BackendImpl::OnEntryDestroyBegin(Addr address) {
  EntriesMap::iterator it = open_entries_.find(address.value());
  if (it != open_entries_.end())
    open_entries_.erase(it);
}
