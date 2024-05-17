void GDataCache::Remove(const std::string& resource_id,
                        base::PlatformFileError* error) {
  AssertOnSequencedWorkerPool();
  DCHECK(error);

  scoped_ptr<CacheEntry> cache_entry =
      GetCacheEntry(resource_id, std::string());

  if (!cache_entry.get() ||
      cache_entry->IsDirty() ||
      cache_entry->IsMounted()) {
    DVLOG(1) << "Entry is "
             << (cache_entry.get() ?
                 (cache_entry->IsDirty() ? "dirty" : "mounted") :
                 "non-existent")
             << " in cache, not removing";
    *error = base::PLATFORM_FILE_OK;
    return;
  }

  std::vector<FilePath> paths_to_delete;

  paths_to_delete.push_back(GetCacheFilePath(resource_id,
                                             util::kWildCard,
                                             CACHE_TYPE_PERSISTENT,
                                             CACHED_FILE_FROM_SERVER));
  paths_to_delete.push_back(GetCacheFilePath(resource_id,
                                             util::kWildCard,
                                             CACHE_TYPE_TMP,
                                             CACHED_FILE_FROM_SERVER));

  paths_to_delete.push_back(GetCacheFilePath(resource_id,
                                             std::string(),
                                             CACHE_TYPE_PINNED,
                                             CACHED_FILE_FROM_SERVER));

  FilePath path_to_keep = GetCacheFilePath(resource_id,
                                           std::string(),
                                           CACHE_TYPE_PERSISTENT,
                                           CACHED_FILE_LOCALLY_MODIFIED);

  for (size_t i = 0; i < paths_to_delete.size(); ++i) {
    DeleteFilesSelectively(paths_to_delete[i], path_to_keep);
  }

  metadata_->RemoveFromCache(resource_id);

  *error = base::PLATFORM_FILE_OK;
}
