  void PrepareForInitCacheTest() {
    ASSERT_TRUE(file_util::CreateDirectory(
        file_system_->cache_paths_[GDataRootDirectory::CACHE_TYPE_PERSISTENT]));
    ASSERT_TRUE(file_util::CreateDirectory(
        file_system_->cache_paths_[GDataRootDirectory::CACHE_TYPE_TMP]));
    ASSERT_TRUE(file_util::CreateDirectory(
        file_system_->cache_paths_[GDataRootDirectory::CACHE_TYPE_PINNED]));
    ASSERT_TRUE(file_util::CreateDirectory(
        file_system_->cache_paths_[GDataRootDirectory::CACHE_TYPE_OUTGOING]));


    for (size_t i = 0; i < ARRAYSIZE_UNSAFE(initial_cache_resources); ++i) {
      const struct InitialCacheResource& resource = initial_cache_resources[i];
      FilePath dest_path = file_system_->GetCacheFilePath(
          resource.resource_id,
          resource.md5,
          GDataFile::IsCachePinned(resource.cache_state) ||
              GDataFile::IsCacheDirty(resource.cache_state) ?
                  GDataRootDirectory::CACHE_TYPE_PERSISTENT :
                  GDataRootDirectory::CACHE_TYPE_TMP,
          GDataFile::IsCacheDirty(resource.cache_state) ?
              GDataFileSystem::CACHED_FILE_LOCALLY_MODIFIED :
              GDataFileSystem::CACHED_FILE_FROM_SERVER);

      if (GDataFile::IsCachePresent(resource.cache_state)) {
        FilePath source_path = GetTestFilePath(resource.source_file);
        ASSERT_TRUE(file_util::CopyFile(source_path, dest_path));
      } else {
        dest_path = FilePath(FILE_PATH_LITERAL(kSymLinkToDevNull));
      }

      if (GDataFile::IsCachePinned(resource.cache_state)) {
        FilePath link_path = file_system_->GetCacheFilePath(
            resource.resource_id,
            "",
            GDataRootDirectory::CACHE_TYPE_PINNED,
            GDataFileSystem::CACHED_FILE_FROM_SERVER);
        ASSERT_TRUE(file_util::CreateSymbolicLink(dest_path, link_path));
      }

      if (GDataFile::IsCacheDirty(resource.cache_state)) {
        FilePath link_path = file_system_->GetCacheFilePath(
            resource.resource_id,
            "",
            GDataRootDirectory::CACHE_TYPE_OUTGOING,
            GDataFileSystem::CACHED_FILE_FROM_SERVER);
        ASSERT_TRUE(file_util::CreateSymbolicLink(dest_path, link_path));
      }
    }
  }
