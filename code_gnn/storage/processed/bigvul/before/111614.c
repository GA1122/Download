  void VerifyRemoveFromCache(base::PlatformFileError error,
                             const std::string& resource_id,
                             const std::string& md5) {
    ++num_callback_invocations_;

    EXPECT_EQ(expected_error_, error);

    GDataRootDirectory::CacheEntry* entry =
        file_system_->root_->GetCacheEntry(resource_id, md5);
    if (entry)
      EXPECT_TRUE(entry->IsDirty());

    std::vector<PathToVerify> paths_to_verify;
    paths_to_verify.push_back(   
        PathToVerify(file_system_->GetCacheFilePath(resource_id, "*",
                     GDataRootDirectory::CACHE_TYPE_TMP,
                     GDataFileSystem::CACHED_FILE_FROM_SERVER), FilePath()));
    paths_to_verify.push_back(   
        PathToVerify(file_system_->GetCacheFilePath(resource_id, "*",
                     GDataRootDirectory::CACHE_TYPE_PERSISTENT,
                     GDataFileSystem::CACHED_FILE_FROM_SERVER), FilePath()));
    paths_to_verify.push_back(   
        PathToVerify(file_system_->GetCacheFilePath(resource_id, "",
                     GDataRootDirectory::CACHE_TYPE_PINNED,
                     GDataFileSystem::CACHED_FILE_FROM_SERVER), FilePath()));
    paths_to_verify.push_back(   
        PathToVerify(file_system_->GetCacheFilePath(resource_id, "",
                     GDataRootDirectory::CACHE_TYPE_OUTGOING,
                     GDataFileSystem::CACHED_FILE_FROM_SERVER), FilePath()));
    if (!entry) {
      for (size_t i = 0; i < paths_to_verify.size(); ++i) {
        file_util::FileEnumerator enumerator(
            paths_to_verify[i].path_to_scan.DirName(), false  ,
            static_cast<file_util::FileEnumerator::FileType>(
                file_util::FileEnumerator::FILES |
                file_util::FileEnumerator::SHOW_SYM_LINKS),
            paths_to_verify[i].path_to_scan.BaseName().value());
        EXPECT_TRUE(enumerator.Next().empty());
      }
    } else {

      paths_to_verify[1].expected_existing_path =
          GetCacheFilePath(resource_id,
                           std::string(),
                           GDataRootDirectory::CACHE_TYPE_PERSISTENT,
                           GDataFileSystem::CACHED_FILE_LOCALLY_MODIFIED);

      paths_to_verify[3].expected_existing_path =
          GetCacheFilePath(resource_id,
                           std::string(),
                           GDataRootDirectory::CACHE_TYPE_OUTGOING,
                           GDataFileSystem::CACHED_FILE_FROM_SERVER);

      if (entry->IsPinned()) {
         paths_to_verify[2].expected_existing_path =
             GetCacheFilePath(resource_id,
                              std::string(),
                              GDataRootDirectory::CACHE_TYPE_PINNED,
                              GDataFileSystem::CACHED_FILE_FROM_SERVER);
      }

      for (size_t i = 0; i < paths_to_verify.size(); ++i) {
        const struct PathToVerify& verify = paths_to_verify[i];
        file_util::FileEnumerator enumerator(
            verify.path_to_scan.DirName(), false  ,
            static_cast<file_util::FileEnumerator::FileType>(
                file_util::FileEnumerator::FILES |
                file_util::FileEnumerator::SHOW_SYM_LINKS),
            verify.path_to_scan.BaseName().value());
        size_t num_files_found = 0;
        for (FilePath current = enumerator.Next(); !current.empty();
             current = enumerator.Next()) {
          ++num_files_found;
          EXPECT_EQ(verify.expected_existing_path, current);
        }
        if (verify.expected_existing_path.empty())
          EXPECT_EQ(0U, num_files_found);
        else
          EXPECT_EQ(1U, num_files_found);
      }
    }
  }
