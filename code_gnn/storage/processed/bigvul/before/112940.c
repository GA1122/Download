void OnGetFileInfoForInsertGDataCachePathsPermissions(
    Profile* profile,
    std::vector<std::pair<FilePath, int> >* cache_paths,
    const base::Closure& callback,
    base::PlatformFileError error,
    scoped_ptr<GDataFileProto> file_info) {
  DCHECK(profile);
  DCHECK(cache_paths);
  DCHECK(!callback.is_null());

  GDataCache* cache = GetGDataCache(profile);
  if (!cache || error != base::PLATFORM_FILE_OK) {
    callback.Run();
    return;
  }

  DCHECK(file_info.get());
  std::string resource_id = file_info->gdata_entry().resource_id();
  std::string file_md5 = file_info->file_md5();

  cache_paths->push_back(std::make_pair(
      cache->GetCacheFilePath(resource_id, file_md5,
          GDataCache::CACHE_TYPE_PERSISTENT,
          GDataCache::CACHED_FILE_FROM_SERVER),
      kReadOnlyFilePermissions));
  cache_paths->push_back(std::make_pair(
      cache->GetCacheFilePath(resource_id, file_md5,
          GDataCache::CACHE_TYPE_PERSISTENT,
          GDataCache::CACHED_FILE_LOCALLY_MODIFIED),
     kReadOnlyFilePermissions));
  cache_paths->push_back(std::make_pair(
      cache->GetCacheFilePath(resource_id, file_md5,
          GDataCache::CACHE_TYPE_PERSISTENT,
          GDataCache::CACHED_FILE_MOUNTED),
     kReadOnlyFilePermissions));
  cache_paths->push_back(std::make_pair(
      cache->GetCacheFilePath(resource_id, file_md5,
          GDataCache::CACHE_TYPE_TMP,
          GDataCache::CACHED_FILE_FROM_SERVER),
      kReadOnlyFilePermissions));

  callback.Run();
}
