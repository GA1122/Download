  void VerifySetMountedState(const std::string& resource_id,
                             const std::string& md5,
                             bool to_mount,
                             base::PlatformFileError error,
                             const FilePath& file_path) {
    ++num_callback_invocations_;
    EXPECT_TRUE(file_util::PathExists(file_path));
    EXPECT_TRUE(file_path == file_system_->GetCacheFilePath(
        resource_id,
        md5,
        expected_sub_dir_type_,
        to_mount ?
            GDataFileSystemInterface::CACHED_FILE_MOUNTED :
            GDataFileSystemInterface::CACHED_FILE_FROM_SERVER));
  }
