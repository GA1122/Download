void CollectBacklog(std::vector<std::string>* to_fetch,
                    std::vector<std::string>* to_upload,
                    const std::string& resource_id,
                    const GDataCache::CacheEntry& cache_entry) {
  DCHECK(to_fetch);
  DCHECK(to_upload);

  if (cache_entry.IsPinned() && !cache_entry.IsPresent())
    to_fetch->push_back(resource_id);

  if (cache_entry.IsDirty())
    to_upload->push_back(resource_id);
}
