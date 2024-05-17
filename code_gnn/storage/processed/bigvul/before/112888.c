void GDataCache::MarkDirty(const std::string& resource_id,
                           const std::string& md5,
                           FileOperationType file_operation_type,
                           base::PlatformFileError* error,
                           FilePath* cache_file_path) {
  AssertOnSequencedWorkerPool();
  DCHECK(error);
  DCHECK(cache_file_path);

  scoped_ptr<CacheEntry> cache_entry =
      GetCacheEntry(resource_id, std::string());

  if (!cache_entry.get() ||
      cache_entry->sub_dir_type == CACHE_TYPE_PINNED) {
    LOG(WARNING) << "Can't mark dirty a file that wasn't cached: res_id="
                 << resource_id
                 << ", md5=" << md5;
    *error = base::PLATFORM_FILE_ERROR_NOT_FOUND;
    return;
  }

  if (cache_entry->IsDirty()) {
    DCHECK_EQ(CACHE_TYPE_PERSISTENT, cache_entry->sub_dir_type);

    FilePath symlink_path = GetCacheFilePath(
        resource_id,
        std::string(),
        CACHE_TYPE_OUTGOING,
        CACHED_FILE_FROM_SERVER);

    *error = ModifyCacheState(
        FilePath(),   
        FilePath(),   
        file_operation_type,
        symlink_path,
        false  );

    if (*error == base::PLATFORM_FILE_OK) {
      *cache_file_path = GetCacheFilePath(
          resource_id,
          md5,
          CACHE_TYPE_PERSISTENT,
          CACHED_FILE_LOCALLY_MODIFIED);
    }
    return;
  }


  FilePath source_path = GetCacheFilePath(
      resource_id,
      md5,
      cache_entry->sub_dir_type,
      CACHED_FILE_FROM_SERVER);

  CacheSubDirectoryType sub_dir_type = CACHE_TYPE_PERSISTENT;
  *cache_file_path = GetCacheFilePath(resource_id,
                                      md5,
                                      sub_dir_type,
                                      CACHED_FILE_LOCALLY_MODIFIED);

  FilePath symlink_path;
  if (cache_entry->IsPinned()) {
    symlink_path = GetCacheFilePath(resource_id,
                                    std::string(),
                                    CACHE_TYPE_PINNED,
                                    CACHED_FILE_FROM_SERVER);
  }

  *error = ModifyCacheState(
      source_path,
      *cache_file_path,
      file_operation_type,
      symlink_path,
      !symlink_path.empty()  );

  if (*error == base::PLATFORM_FILE_OK) {
    int cache_state = SetCacheDirty(cache_entry->cache_state);
    metadata_->UpdateCache(resource_id, md5, sub_dir_type, cache_state);
  }
}
