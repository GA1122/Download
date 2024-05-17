void BackendImpl::InternalDoomEntry(EntryImpl* entry) {
  uint32_t hash = entry->GetHash();
  std::string key = entry->GetKey();
  Addr entry_addr = entry->entry()->address();
  bool error;
  scoped_refptr<EntryImpl> parent_entry =
      MatchEntry(key, hash, true, entry_addr, &error);
  CacheAddr child(entry->GetNextAddress());

  Trace("Doom entry 0x%p", entry);

  if (!entry->doomed()) {
    eviction_.OnDoomEntry(entry);
    entry->InternalDoom();
    if (!new_eviction_) {
      DecreaseNumEntries();
    }
    stats_.OnEvent(Stats::DOOM_ENTRY);
  }

  if (parent_entry) {
    parent_entry->SetNextAddress(Addr(child));
    parent_entry = nullptr;
  } else if (!error) {
    data_->table[hash & mask_] = child;
  }

  FlushIndex();
}
