  FilePath GetCacheFilePath(
      const std::string& resource_id,
      const std::string& md5,
      GDataRootDirectory::CacheSubDirectoryType sub_dir_type,
      GDataFileSystem::CachedFileOrigin file_origin) {
    return file_system_->GetCacheFilePath(resource_id, md5, sub_dir_type,
                                          file_origin);
  }
