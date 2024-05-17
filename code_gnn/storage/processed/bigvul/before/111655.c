void GDataRootDirectory::RemoveTemporaryFilesFromCacheMap() {
  CacheMap::iterator iter = cache_map_.begin();
  while (iter != cache_map_.end()) {
    CacheEntry* entry = iter->second;
    if (entry->sub_dir_type == CACHE_TYPE_TMP) {
      delete entry;
      cache_map_.erase(iter++);
    } else {
      ++iter;
    }
  }
}
