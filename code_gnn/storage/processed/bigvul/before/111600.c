  void TestInitializeCache() {
    for (size_t i = 0; i < ARRAYSIZE_UNSAFE(initial_cache_resources); ++i) {
      const struct InitialCacheResource& resource = initial_cache_resources[i];
      num_callback_invocations_ = 0;
      TestGetFileFromCacheByResourceIdAndMd5(
          resource.resource_id,
          resource.md5,
          GDataFile::IsCachePresent(resource.cache_state) ?
          base::PLATFORM_FILE_OK :
          base::PLATFORM_FILE_ERROR_NOT_FOUND,
          resource.expected_file_extension);
      EXPECT_EQ(1, num_callback_invocations_);

      std::string md5;
      if (GDataFile::IsCachePresent(resource.cache_state))
         md5 = resource.md5;
      GDataRootDirectory::CacheEntry* entry =
          file_system_->root_->GetCacheEntry(resource.resource_id, md5);
      ASSERT_TRUE(entry != NULL);
      EXPECT_EQ(resource.cache_state, entry->cache_state);
      EXPECT_EQ(resource.expected_sub_dir_type, entry->sub_dir_type);
    }
  }
