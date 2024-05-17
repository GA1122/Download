CacheAddr BackendImpl::GetNextAddr(Addr address) {
  EntriesMap::iterator it = open_entries_.find(address.value());
  if (it != open_entries_.end()) {
    EntryImpl* this_entry = it->second;
    return this_entry->GetNextAddress();
  }
  DCHECK(block_files_.IsValid(address));
  DCHECK(!address.is_separate_file() && address.file_type() == BLOCK_256);

  CacheEntryBlock entry(File(address), address);
  CHECK(entry.Load());
  return entry.Data()->next;
}
