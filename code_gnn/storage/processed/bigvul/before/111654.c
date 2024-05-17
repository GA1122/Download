void GDataRootDirectory::RemoveFromCacheMap(const std::string& resource_id) {

  CacheMap::iterator iter = cache_map_.find(resource_id);
  if (iter != cache_map_.end()) {
    delete iter->second;
    cache_map_.erase(iter);
  }
}
