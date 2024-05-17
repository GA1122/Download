void CollectExistingPinnedFile(std::vector<std::string>* resource_ids,
                               const std::string& resource_id,
                               const GDataCache::CacheEntry& cache_entry) {
  DCHECK(resource_ids);

  if (cache_entry.IsPinned() && cache_entry.IsPresent())
    resource_ids->push_back(resource_id);
}
