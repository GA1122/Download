void BackendImpl::RecoveredEntry(CacheRankingsBlock* rankings) {
  Addr address(rankings->Data()->contents);
  scoped_refptr<EntryImpl> cache_entry;
  if (NewEntry(address, &cache_entry)) {
    STRESS_NOTREACHED();
    return;
  }

  uint32_t hash = cache_entry->GetHash();
  cache_entry = nullptr;

  if (data_->table[hash & mask_])
    return;

  data_->table[hash & mask_] = address.value();
  FlushIndex();
}
