  void TestGetFileFromCacheByResourceIdAndMd5(
      const std::string& resource_id,
      const std::string& md5,
      base::PlatformFileError expected_error,
      const std::string& expected_file_extension) {
    expected_error_ = expected_error;
    expected_file_extension_ = expected_file_extension;

    file_system_->GetFileFromCacheByResourceIdAndMd5(resource_id, md5,
        base::Bind(&GDataFileSystemTest::VerifyGetFromCache,
                   base::Unretained(this)));

    RunAllPendingForIO();
  }
