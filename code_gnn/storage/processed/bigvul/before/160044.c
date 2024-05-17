 EntryImpl* BackendImpl::GetOpenEntry(CacheRankingsBlock* rankings) const {
  DCHECK(rankings->HasData());
  EntriesMap::const_iterator it =
      open_entries_.find(rankings->Data()->contents);
  if (it != open_entries_.end()) {
    return it->second;
  }

  return NULL;
}
