void BackendImpl::SyncOnExternalCacheHit(const std::string& key) {
  if (disabled_)
    return;

  uint32_t hash = base::Hash(key);
  bool error;
  scoped_refptr<EntryImpl> cache_entry =
      MatchEntry(key, hash, false, Addr(), &error);
  if (cache_entry && ENTRY_NORMAL == cache_entry->entry()->Data()->state)
    UpdateRank(cache_entry.get(), cache_type() == net::SHADER_CACHE);
}
