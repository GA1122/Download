bool GDataCacheMetadataMap::CheckIfMd5Matches(
    const std::string& md5,
    const GDataCache::CacheEntry& cache_entry) {
  if (cache_entry.IsDirty()) {
    return true;
  } else if (cache_entry.IsPinned() && cache_entry.md5.empty()) {
    return true;
  } else if (md5.empty()) {
    return true;
  } else if (md5 == cache_entry.md5) {
    return true;
  }
  return false;
}
