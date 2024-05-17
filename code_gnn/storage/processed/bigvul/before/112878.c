void GDataCache::GetFile(const std::string& resource_id,
                         const std::string& md5,
                         base::PlatformFileError* error,
                         FilePath* cache_file_path) {
  AssertOnSequencedWorkerPool();
  DCHECK(error);
  DCHECK(cache_file_path);

  scoped_ptr<CacheEntry> cache_entry = GetCacheEntry(
      resource_id, md5);
  if (cache_entry.get() && cache_entry->IsPresent()) {
    CachedFileOrigin file_origin;
    if (cache_entry->IsMounted()) {
      file_origin = CACHED_FILE_MOUNTED;
    } else if (cache_entry->IsDirty()) {
      file_origin = CACHED_FILE_LOCALLY_MODIFIED;
    } else {
      file_origin = CACHED_FILE_FROM_SERVER;
    }
    *cache_file_path = GetCacheFilePath(
        resource_id,
        md5,
        cache_entry->sub_dir_type,
        file_origin);
    *error = base::PLATFORM_FILE_OK;
  } else {
    *error = base::PLATFORM_FILE_ERROR_NOT_FOUND;
  }
}
