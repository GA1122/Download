void GDataCache::SetMountedState(const FilePath& file_path,
                                 bool to_mount,
                                 base::PlatformFileError *error,
                                 FilePath* cache_file_path) {
  AssertOnSequencedWorkerPool();
  DCHECK(error);
  DCHECK(cache_file_path);

  std::string resource_id;
  std::string md5;
  std::string extra_extension;
  util::ParseCacheFilePath(file_path, &resource_id, &md5, &extra_extension);
  DCHECK(!to_mount == (extra_extension == util::kMountedArchiveFileExtension));

  scoped_ptr<CacheEntry> cache_entry = GetCacheEntry(
      resource_id, md5);
  if (!cache_entry.get()) {
    *error = base::PLATFORM_FILE_ERROR_NOT_FOUND;
    return;
  }
  if (to_mount == cache_entry->IsMounted()) {
    *error = base::PLATFORM_FILE_ERROR_INVALID_OPERATION;
    return;
  }

  CacheSubDirectoryType unmounted_subdir =
      cache_entry->IsPinned() ? CACHE_TYPE_PERSISTENT : CACHE_TYPE_TMP;
  FilePath unmounted_path = GetCacheFilePath(
      resource_id, md5, unmounted_subdir, CACHED_FILE_FROM_SERVER);

  CacheSubDirectoryType mounted_subdir = CACHE_TYPE_PERSISTENT;
  FilePath mounted_path = GetCacheFilePath(
      resource_id, md5, mounted_subdir, CACHED_FILE_MOUNTED);

  FilePath source_path;
  CacheSubDirectoryType dest_subdir;
  int cache_state = cache_entry->cache_state;
  if (to_mount) {
    source_path = unmounted_path;
    *cache_file_path = mounted_path;
    dest_subdir = mounted_subdir;
    cache_state = SetCacheMounted(cache_state);
  } else {
    source_path = mounted_path;
    *cache_file_path = unmounted_path;
    dest_subdir = unmounted_subdir;
    cache_state = ClearCacheMounted(cache_state);
  }

  *error = ModifyCacheState(source_path, *cache_file_path,
                            FILE_OPERATION_MOVE, FilePath(), false);
  if (*error == base::PLATFORM_FILE_OK) {
    metadata_->UpdateCache(resource_id, md5, dest_subdir, cache_state);
  }
}
