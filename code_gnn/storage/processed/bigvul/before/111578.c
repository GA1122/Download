  FilePath GetCachePathForFile(GDataFile* file) {
    return file_system_->GetCacheFilePath(
        file->resource_id(),
        file->file_md5(),
        GDataRootDirectory::CACHE_TYPE_TMP,
        GDataFileSystem::CACHED_FILE_FROM_SERVER);
  }
