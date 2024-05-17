void GDataCache::Unpin(const std::string& resource_id,
                       const std::string& md5,
                       FileOperationType file_operation_type,
                       base::PlatformFileError* error) {
  AssertOnSequencedWorkerPool();
  DCHECK(error);

  scoped_ptr<CacheEntry> cache_entry = GetCacheEntry(resource_id, md5);

  if (!cache_entry.get()) {
    LOG(WARNING) << "Can't unpin a file that wasn't pinned or cached: res_id="
                 << resource_id
                 << ", md5=" << md5;
    *error = base::PLATFORM_FILE_ERROR_NOT_FOUND;
    return;
  }


  FilePath source_path;
  FilePath dest_path;
  CacheSubDirectoryType sub_dir_type = CACHE_TYPE_TMP;

  if (cache_entry->IsDirty() || cache_entry->IsMounted()) {
    sub_dir_type = CACHE_TYPE_PERSISTENT;
    DCHECK_EQ(sub_dir_type, cache_entry->sub_dir_type);
    dest_path = GetCacheFilePath(resource_id,
                                 md5,
                                 cache_entry->sub_dir_type,
                                 CACHED_FILE_LOCALLY_MODIFIED);
    source_path = dest_path;
  } else {
    source_path = GetCacheFilePath(resource_id,
                                   md5,
                                   cache_entry->sub_dir_type,
                                   CACHED_FILE_FROM_SERVER);

    if (cache_entry->sub_dir_type == CACHE_TYPE_PINNED) {
      dest_path = source_path;
    } else {   
      dest_path = GetCacheFilePath(resource_id, md5,
                                   CACHE_TYPE_TMP,
                                   CACHED_FILE_FROM_SERVER);
    }
  }

  FilePath symlink_path;
  if (cache_entry->IsPinned()) {
    symlink_path = GetCacheFilePath(resource_id,
                                    std::string(),
                                    CACHE_TYPE_PINNED,
                                    CACHED_FILE_FROM_SERVER);
  }

  *error = ModifyCacheState(
      source_path,
      dest_path,
      file_operation_type,
      symlink_path,   
      false  );

  if (*error == base::PLATFORM_FILE_OK) {
    int cache_state = ClearCachePinned(cache_entry->cache_state);
    metadata_->UpdateCache(resource_id, md5, sub_dir_type, cache_state);
  }
}
