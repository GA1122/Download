  void TestSetMountedState(
      const std::string& resource_id,
      const std::string& md5,
      const FilePath& file_path,
      bool to_mount,
      base::PlatformFileError expected_error,
      int expected_cache_state,
      GDataRootDirectory::CacheSubDirectoryType expected_sub_dir_type) {
    expected_error_ = expected_error;
    expected_cache_state_ = expected_cache_state;
    expected_sub_dir_type_ = expected_sub_dir_type;
    expect_outgoing_symlink_ = false;

    file_system_->SetMountedState(file_path, to_mount,
        base::Bind(&GDataFileSystemTest::VerifySetMountedState,
                   base::Unretained(this), resource_id, md5, to_mount));

    RunAllPendingForIO();
  }
