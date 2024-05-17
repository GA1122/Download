void GDataCache::GetCacheEntryHelper(const std::string& resource_id,
                                     const std::string& md5,
                                     bool* success,
                                     GDataCache::CacheEntry* cache_entry) {
  AssertOnSequencedWorkerPool();
  DCHECK(success);
  DCHECK(cache_entry);

  scoped_ptr<GDataCache::CacheEntry> value(GetCacheEntry(resource_id, md5));
  *success = value.get();
  if (*success)
    *cache_entry = *value;
}
