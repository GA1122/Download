  void TestGetCacheFilePath(const std::string& resource_id,
                            const std::string& md5,
                            const std::string& expected_filename) {
    FilePath actual_path = file_system_->GetCacheFilePath(
        resource_id,
        md5,
        GDataRootDirectory::CACHE_TYPE_TMP,
        GDataFileSystem::CACHED_FILE_FROM_SERVER);
    FilePath expected_path =
        file_system_->cache_paths_[GDataRootDirectory::CACHE_TYPE_TMP];
    expected_path = expected_path.Append(expected_filename);
    EXPECT_EQ(expected_path, actual_path);

    FilePath base_name = actual_path.BaseName();

    std::string unescaped_md5 = util::UnescapeCacheFileName(
        base_name.Extension().substr(1));
    EXPECT_EQ(md5, unescaped_md5);
    std::string unescaped_resource_id = util::UnescapeCacheFileName(
        base_name.RemoveExtension().value());
    EXPECT_EQ(resource_id, unescaped_resource_id);
  }
