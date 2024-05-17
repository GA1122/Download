void GDataCache::Store(const std::string& resource_id,
                       const std::string& md5,
                       const FilePath& source_path,
                       FileOperationType file_operation_type,
                       base::PlatformFileError* error) {
  AssertOnSequencedWorkerPool();
  DCHECK(error);

  FilePath dest_path;
  FilePath symlink_path;
  int cache_state = CACHE_STATE_PRESENT;
  CacheSubDirectoryType sub_dir_type = CACHE_TYPE_TMP;

  scoped_ptr<CacheEntry> cache_entry = GetCacheEntry(resource_id, md5);

  if (cache_entry.get()) {   
    if (cache_entry->IsDirty() || cache_entry->IsMounted()) {
      LOG(WARNING) << "Can't store a file to replace a "
                   << (cache_entry->IsDirty() ? "dirty" : "mounted")
                   << " file: res_id=" << resource_id
                   << ", md5=" << md5;
      *error = base::PLATFORM_FILE_ERROR_IN_USE;
      return;
    }

    cache_state |= cache_entry->cache_state;

    if (cache_entry->IsPinned()) {
      sub_dir_type = CACHE_TYPE_PERSISTENT;
      dest_path = GetCacheFilePath(resource_id, md5, sub_dir_type,
                                   CACHED_FILE_FROM_SERVER);
      symlink_path = GetCacheFilePath(
          resource_id, std::string(), CACHE_TYPE_PINNED,
          CACHED_FILE_FROM_SERVER);
    }
  }

  if (dest_path.empty()) {
    DCHECK_EQ(CACHE_TYPE_TMP, sub_dir_type);
    dest_path = GetCacheFilePath(resource_id, md5, sub_dir_type,
                                 CACHED_FILE_FROM_SERVER);
  }

  *error = ModifyCacheState(
      source_path,
      dest_path,
      file_operation_type,
      symlink_path,
      !symlink_path.empty());   

  FilePath stale_filenames_pattern;
  if (md5.empty()) {
    stale_filenames_pattern = FilePath(dest_path.value() + util::kWildCard);
  } else {
    stale_filenames_pattern = dest_path.ReplaceExtension(util::kWildCard);
  }

  DeleteFilesSelectively(stale_filenames_pattern, dest_path);

  if (*error == base::PLATFORM_FILE_OK) {
    metadata_->UpdateCache(resource_id, md5, sub_dir_type, cache_state);
  }
}
