void GDataCacheMetadataMap::RemoveTemporaryFiles() {
  AssertOnSequencedWorkerPool();

  CacheMap::iterator iter = cache_map_.begin();
  while (iter != cache_map_.end()) {
    if (iter->second.sub_dir_type == GDataCache::CACHE_TYPE_TMP) {
      cache_map_.erase(iter++);
    } else {
      ++iter;
    }
  }
}
