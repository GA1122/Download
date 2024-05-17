  bool CacheFileExists(const std::string& resource_id,
                       const std::string& md5) {
    const FilePath file_path = file_system_->GetCacheFilePath(
        resource_id,
        md5,
        GDataRootDirectory::CACHE_TYPE_TMP,
        GDataFileSystem::CACHED_FILE_FROM_SERVER);
    return file_util::PathExists(file_path);
  }
