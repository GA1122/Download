  void VerifyMarkDirty(base::PlatformFileError error,
                       const std::string& resource_id,
                       const std::string& md5,
                       const FilePath& cache_file_path) {
    VerifyCacheFileState(error, resource_id, md5);

    if (error == base::PLATFORM_FILE_OK) {
      FilePath base_name = cache_file_path.BaseName();
      EXPECT_EQ(util::EscapeCacheFileName(resource_id) +
                FilePath::kExtensionSeparator +
                "local",
                base_name.value());
    } else {
      EXPECT_TRUE(cache_file_path.empty());
    }
  }
