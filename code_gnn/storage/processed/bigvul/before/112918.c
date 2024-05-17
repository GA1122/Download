scoped_ptr<GDataCache::CacheEntry> GDataCacheMetadataMap::GetCacheEntry(
    const std::string& resource_id,
    const std::string& md5) {
  AssertOnSequencedWorkerPool();

  CacheMap::iterator iter = cache_map_.find(resource_id);
  if (iter == cache_map_.end()) {
    DVLOG(1) << "Can't find " << resource_id << " in cache map";
    return scoped_ptr<GDataCache::CacheEntry>();
  }

  scoped_ptr<GDataCache::CacheEntry> cache_entry(
      new GDataCache::CacheEntry(iter->second));

  if (!CheckIfMd5Matches(md5, *cache_entry)) {
    DVLOG(1) << "Non-matching md5: want=" << md5
             << ", found=[res_id=" << resource_id
             << ", " << cache_entry->ToString()
             << "]";
    return scoped_ptr<GDataCache::CacheEntry>();
  }

  DVLOG(1) << "Found entry for res_id=" << resource_id
           << ", " << cache_entry->ToString();

  return cache_entry.Pass();
}
