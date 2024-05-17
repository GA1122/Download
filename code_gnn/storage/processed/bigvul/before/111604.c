  void TestRemoveFromCache(const std::string& resource_id,
                           base::PlatformFileError expected_error) {
    expected_error_ = expected_error;

    file_system_->RemoveFromCache(resource_id,
        base::Bind(&GDataFileSystemTest::VerifyRemoveFromCache,
                   base::Unretained(this)));

    RunAllPendingForIO();
  }
