scoped_ptr<GDataCache::CacheEntry> GDataCache::GetCacheEntry(
    const std::string& resource_id,
    const std::string& md5) {
  AssertOnSequencedWorkerPool();
  return metadata_->GetCacheEntry(resource_id, md5);
}
