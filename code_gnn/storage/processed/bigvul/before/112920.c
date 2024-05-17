void GDataCacheMetadataMap::RemoveFromCache(const std::string& resource_id) {
  AssertOnSequencedWorkerPool();

  CacheMap::iterator iter = cache_map_.find(resource_id);
  if (iter != cache_map_.end()) {
    cache_map_.erase(iter);
  }
}
